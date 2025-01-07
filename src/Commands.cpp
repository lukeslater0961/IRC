#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include <sys/socket.h>


void	SetUsername(std::vector<std::string> tokens, Client *client)
{
	if (tokens.size() == 1)
	{
		SendErrorMsg("USER :", USER_USAGE, client);
		return ;
	}
	else
		client->setUsername(tokens[1]);
}

bool	CheckClientPass(Client *client)
{
	if (client)
	{
		if (client->GetPassword())
		{
			SendErrorMsg("462 ", ALR_REGISTERED, client);
			return (true);
		}
	}
	return (false);
}

void	CheckPass(std::vector<std::string> tokens, Client *client, Server *server)
{
	std::string password;
	if (!CheckClientPass(client))
	{
		if (tokens.size() == 1)
		{
			SendErrorMsg("461 PASS", PASS_INC_PARAM, client);
			send(client->GetSocket(), PASS_USAGE, sizeof(PASS_USAGE), MSG_EOR);
			return ;
		}
		password = tokens[1];
		if (tokens.size() > 2)
		{
			SendErrorMsg("464 PASS", PASS_INC, client);
			return;
		}
		else
		{
			if (!std::strcmp(password.c_str(), server->GetPassword().c_str()))
				client->SetPassword(true);
			else 
				SendErrorMsg("464 PASS", PASS_INC, client);
		}
	}
}

void CheckNickname(std::vector<std::string> tokens, Client *client, Server *server)
{
	std::string invChars[] = {":", ",", "*", "@", "!" , "."};
	std::string nick;

	if (tokens.size() == 1)
	{
		SendErrorMsg("Usage: ", NICK_USAGE, client);
		return ;
	}
	
	nick = tokens[1];
	for (size_t i = 0; i < sizeof(invChars)/sizeof(invChars[0]); ++i)
	{
		if (!tokens[1].find(invChars[i]))		
		{
			SendErrorMsg(":localhost 432 " + nick, NON_NICK, client);
			return;
		}
	}

	for (std::vector<Client *>::iterator it = server->client.begin(); it != server->client.end(); it++)
	{
		if ((*it)->getNickname() == nick)
		{
            SendMsg(client, ":localhost 433 * " + nick + " " + NICK_USE);
			return;
		}
	}
	client->setNickname(nick);
    SendMsg(client, ":localhost 001 " + client->getNickname() + " :Registered nickname " + client->getNickname());
}


static Client *FindClientName(std::string name, Server *server)
{
	for (std::vector<Client *>::iterator it = server->client.begin(); it != server->client.end(); it++)
	{
		if ((*it)->getNickname() == name)
			return (*it);
	}
	return (NULL);
}

void KickCommand(Server &server, const std::string &channelName, Client *operatorClient, const std::string &targetNickname)
{
	Channel *channel = server.GetChannel(channelName);
	Client *client = FindClientName(targetNickname, &server);

    if (!channel) {
		SendErrorMsg("403 " + channelName, "No such channel\n", operatorClient);
        return;
    }

    if (!channel->HasOperator(operatorClient->getNickname())) {
		SendErrorMsg("482 " + channelName, "You're not channel operator\n", operatorClient);
        return;
    }

    if (!channel->HasMember(targetNickname)) {
		SendErrorMsg("441 " + targetNickname + " " + channelName, "They aren't on that channel\n", operatorClient);
        return;
    }

    channel->RemoveMember(targetNickname);
    std::cout << "Client " << targetNickname << " has been kicked from channel " << channelName << "." << std::endl;
    std::string msg = ":" + operatorClient->getNickname() + "!" + operatorClient->getUsername() + "@localhost KICK " + channelName + " " + targetNickname + "\n";
    channel->broadcast(msg, NULL);
	SendMsg(client, msg);
	client->inChannel = false;
}


void InviteCommand(Server &server, const std::string &channelName, Client *operatorClient, std::string target)
{
    Channel *channel = server.GetChannel(channelName);
    Client *targetClient = FindClientName(target, &server);

    if (!targetClient) {
        SendErrorMsg("401 " + target, "No such client", operatorClient);
        return;
    }
    if (!channel) {
		SendErrorMsg("403 " + channelName, "No such channel", operatorClient);
        return;
    }

    if (!channel->HasOperator(operatorClient->getNickname())) {
		SendErrorMsg("482 " + channelName, "You're not channel operator", operatorClient);
        return;
    }

    if (channel->HasMember(target)) {
		SendErrorMsg("443 " + target + " " + channelName, "is already on channel", operatorClient);
        return;
    }
    std::cout << "Client " << target << " has been invited to channel " << channelName << "." << std::endl;
    std::string msg = ":" + operatorClient->getNickname() + "!" + operatorClient->getUsername() + "@localhost INVITE " + target + " " + channelName + "\n";
    channel->broadcast(msg, targetClient);
	std::string inviteMsg = ":" + operatorClient->getNickname() + "!" + operatorClient->getUsername() + "@localhost INVITE " + target + " :" + channelName;
	SendMsg(targetClient, inviteMsg);
	channel->AddToInvited(targetClient);	
}


void TopicCommand(Server &server, const std::string &channelName, Client *operatorClient, std::vector<std::string> &tokens)
{
     Channel *channel = server.GetChannel(channelName);
    std::string newTopic = "";
    if (!channel) {
		SendErrorMsg("403 " + channelName, "No such channel", operatorClient);
        return;
    }
	if (channel->isTopicRestricted())
	{
    	if (!channel->HasOperator(operatorClient->getNickname())) {
			SendErrorMsg("482 " + operatorClient->getNickname() + " " + channelName, "You're not channel operator", operatorClient);
    	    return;
    	}
	}
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) 
    {
        if (it == tokens.begin() || it == tokens.begin() + 1)
            continue;
        newTopic += *it + " ";
    }

    channel->SetTopic(newTopic);
    std::cout << "The topic for channel " << channelName << " has been set to: " << newTopic << std::endl;
	channel->broadcast(":" + operatorClient->getNickname() + " TOPIC " + channelName + " " + newTopic + "\n", NULL);
}

