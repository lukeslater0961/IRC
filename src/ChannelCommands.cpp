#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include <sys/socket.h>


void SendMsg(Client *client, const std::string &message) {
    if (!client) {
        std::cerr << "Error: Null client pointer in SendMsg." << std::endl;
        return;
    }

    std::string msgToSend = message + "\r\n"; // Append IRC standard line ending
    int socket = client->GetSocket();         // Get the client's socket

    if (send(socket, msgToSend.c_str(), msgToSend.length(), 0) == -1) {
        std::cerr << "Error: Failed to send message to client " << client->getNickname() << std::endl;
    }
}

void JoinChannel(std::vector<std::string> tokens, Server *server, Client *client)
{
    if (tokens.size() < 2) {
        SendErrorMsg("461", "JOIN :Not enough parameters", client);
        return;
    }

    std::string channelName = tokens[1];

    // Validate channel name
    if (channelName[0] != '#') {
        SendErrorMsg("403", "JOIN :Invalid channel name", client);
        return;
    }

    Channel *channel = server->GetChannel(channelName);
    if (!channel) {
        server->CreateChannel(channelName);
        channel = server->GetChannel(channelName);
        std::cout << "Channel " << channelName << " created." << std::endl;
    }

    // Check if the client is already a member
    if (channel->HasMember(client->getNickname())) {
        SendErrorMsg("443", channelName + " :You're already in this channel", client);
        return;
    }

    // Add the client to the channel
    channel->AddMember(client);
    client->SetCurrentChannel(channelName);

    // Construct the JOIN confirmation message
    std::string joinMessage = ":" + client->getNickname() + " JOIN " + channelName;

    // Send confirmation to the joining client
    SendMsg(client, joinMessage);

    // Send the current topic of the channel
    if (!channel->GetTopic().empty()) {
        std::string topicMessage = ":localhost 332 " + client->getNickname() + " " + channelName + " :" + channel->GetTopic();
        SendMsg(client, topicMessage);
    }

    // Send a list of current members in the channel
    std::string namesMessage = ":localhost 353 " + client->getNickname() + " = " + channelName + " " + channel->GetMemberList();
    SendMsg(client, namesMessage);

    std::string endOfNamesMessage = ":localhost 366 " + client->getNickname() + " " + channelName + " :End of /NAMES list";
    SendMsg(client, endOfNamesMessage);

    // Broadcast the join message to all other members in the channel
    channel->broadcast(joinMessage, client);
}
