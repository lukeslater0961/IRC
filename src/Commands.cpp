#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"
#include <sys/socket.h>

void	Broadcast(std::vector<std::string> tokens, Server *server, Client *client)
{
	if (tokens.size() == 1)
		return ;
	else if (tokens.size() == 3)
	{
		if (tokens[1][0] == '#')
			BroadcastToChannel(tokens, client, server);
		else
		{
			for (std::vector<Client *>::iterator it = server->client.begin(); it != server->client.end(); it++)
			{
				if ((*it)->getNickname() == tokens[1])
					send((*it)->GetSocket(), tokens[2].c_str(), tokens[2].size(), MSG_EOR);
			}
		}
	}
}


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
    BroadcastToChannel(split(":localhost KICK " + channelName + " " + targetNickname, ' '), operatorClient, &server);
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
}

void TopicCommand(Server &server, const std::string &channelName, Client *operatorClient, std::vector<std::string> &tokens)
{
    Channel *channel = server.GetChannel(channelName);
    std::string newTopic = "";
    if (!channel) {
		SendErrorMsg("403 " + channelName, "No such channel", operatorClient);
        return;
    }

    if (!channel->HasOperator(operatorClient->getNickname())) {
		SendErrorMsg("482 " + channelName, "You're not channel operator", operatorClient);
        return;
    }
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) 
    {
        if (it == tokens.begin() || it == tokens.begin() + 1)
            continue;
        newTopic += *it + " ";
    }
    channel->SetTopic(newTopic);
    std::cout << "The topic for channel " << channelName << " has been set to: " << newTopic << std::endl;
    BroadcastToChannel(split(":localhost TOPIC " + channelName + " " + newTopic, ' '), operatorClient, &server);


}


void ModeCommand(Server &server, Client *operatorClient, const std::string &channelName, const std::string &mode, const std::string &param)
{
    Channel* channel = server.GetChannel(channelName);

    if (!channel) {
        SendErrorMsg("403 " + channelName, "No such channel", operatorClient);
        return;
    }

    if (!channel->HasOperator(operatorClient->getNickname())) {
		SendErrorMsg("482 " + channelName, "You're not channel operator", operatorClient);
        return;
    }

    if (mode == "i") { // Mode invitation uniquement
        if (param == "+i") {
            channel->setInviteOnly(true);
            std::cout << "Channel " << channelName << " is now invite-only." << std::endl;
        } else if (param == "-i") {
            channel->setInviteOnly(false);
            std::cout << "Channel " << channelName << " is no longer invite-only." << std::endl;
        } else {
            SendErrorMsg("472 " + channelName, "Invalid argument for mode i", operatorClient);
        }
    }
    else if (mode == "t") { // Restreindre le sujet aux opérateurs
        if (param == "+t") {
            channel->setTopicRestriction(true);
            std::cout << "Only operators can change the topic in channel " << channelName << "." << std::endl;
        } else if (param == "-t") {
            channel->setTopicRestriction(false);
            std::cout << "All members can change the topic in channel " << channelName << "." << std::endl;
        } else {
            SendErrorMsg("472 " + channelName, "Invalid argument for mode t", operatorClient);
        }
    }
    else if (mode == "k") { // Clé pour le canal
        if (param == "+k") {
            if (param.empty()) {
                SendErrorMsg("461 " + channelName, "You must provide a password for the channel", operatorClient);
                return;
            }
            channel->SetPassword(param);
            std::cout << "Password for channel " << channelName << " set to: " << param << std::endl;
        } else if (param == "-k") {
            channel->SetPassword("");
            std::cout << "Password for channel " << channelName << " removed." << std::endl;
        } else {
            SendErrorMsg("472 " + channelName, "Invalid argument for mode k", operatorClient);
        }
    }
    else if (mode == "l") { // Limite d'utilisateurs
        if (param == "+l") {
            int limit = std::atoi(param.c_str());
            if (limit > 0) {
                channel->setUserLimit(limit);
                std::cout << "User limit for channel " << channelName << " set to: " << limit << std::endl;
            } else {
                SendErrorMsg("461 " + channelName, "You must provide a valid user limit", operatorClient);
            }
        } else if (param == "-l") {
            channel->setUserLimit(0); // No limit
            std::cout << "User limit for channel " << channelName << " removed (no limit)." << std::endl;
        } else {
            SendErrorMsg("472 " + channelName, "Invalid argument for mode l", operatorClient);
        }
    }
    else {
        SendErrorMsg("472 " + channelName, "Invalid mode specified", operatorClient);
    }
}
