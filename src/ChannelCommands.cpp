#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include <sys/socket.h>


void SendMsg(Client *client, const std::string &message)
{
    if (!client) {
        std::cerr << "Error: Null client pointer in SendMsg." << std::endl;
        return;
    }

    std::string msgToSend = message + "\r\n";
    int socket = client->GetSocket();

    if (send(socket, msgToSend.c_str(), msgToSend.length(), 0) == -1)
        std::cerr << "Error: Failed to send message to client " << client->getNickname() << std::endl;
}

int	HandleErrors(Client *client, std::string channelName, Server *server, std::vector<std::string> tokens )
{
	Channel *channel;

   if (channelName.empty() ||  channelName[0] != '#') {
        std::string errorMessage = ":localhost 403 " + client->getNickname() + " " + channelName + " :Invalid channel name";
        SendMsg(client, errorMessage);
        return 1;
    }

    channel = server->GetChannel(channelName);
    if (!channel) {
        server->CreateChannel(channelName);
        channel = server->GetChannel(channelName);
        std::cout << "Channel " << channelName << " created." << std::endl;
    }

    if (channel->isInviteOnly() == 1 && !channel->isInvited(client->getNickname())) {
        SendErrorMsg(channelName, "Cannot join channel (+i)", client);
        return 1;
    }

    if (!channel->GetKey().empty() && tokens.size() < 3)
    {
        if (tokens.size() < 3 || tokens[2] != channel->GetKey()) {
            SendErrorMsg("475", channelName + " :Cannot join channel (+k)", client);
			return 1;
        }
    }else if (tokens.size() == 3)
	{
		if (tokens[2] != channel->GetKey())
		{
			SendErrorMsg("475", channelName + " :Invalid passkey", client);
			return 1;
		}
	}

    if (channel->HasMember(client->getNickname())) {
        std::string errorMessage = ":localhost 443 " + client->getNickname() + " " + channelName + " :is already on channel";
        SendMsg(client, errorMessage);
        return 1;
    }
	return 0;
}

void JoinChannel(std::vector<std::string> tokens, Server *server, Client *client)
{
	Channel *channel;
	std::string joinMessage;

    if (tokens.size() < 2) {
        SendErrorMsg("461", "JOIN :Not enough parameters", client);
        return;
    }

    std::string channelName = tokens[1];
	if (HandleErrors(client, channelName, server, tokens))
		return;
	else
		channel = server->GetChannel(channelName);
   
    channel->AddMember(client);
    client->SetCurrentChannel(channelName);

    joinMessage = ":" + client->getNickname() + " JOIN " + channelName;

    SendMsg(client, joinMessage);

    if (!channel->GetTopic().empty()) {
        std::string topicMessage = ":localhost 332 " + client->getNickname() + " " + channelName + " :" + channel->GetTopic();
        SendMsg(client, topicMessage);
    }

    std::string namesMessage = ":localhost 353 " + client->getNickname() + " = " + channelName + " " + channel->GetMemberList();
    SendMsg(client, namesMessage);

    std::string endOfNamesMessage = ":localhost 366 " + client->getNickname() + " " + channelName + " :End of /NAMES list";
    SendMsg(client, endOfNamesMessage);

    channel->broadcast(joinMessage, client);

    client->inChannel = true;
	channel->RemoveInvitedMember(client->getNickname());
}
