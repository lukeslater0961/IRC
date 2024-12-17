#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include <sys/socket.h>

void	JoinChannel(std::vector<std::string> tokens, Server *server, Client *client)
{
	if (tokens.size() == 1 || tokens.size() > 2)
		SendErrorMsg("Usage: ", JOIN_USAGE, client);
	else
	{
		if (!(tokens[1][0] == '#'))
			SendErrorMsg(tokens[1], CHANNEL_NAME, client);
		else
		{
			std::string message = JOINED + tokens[1] + "\n";
			Channel *channel = server->GetChannel(tokens[1]);
			if (!channel)
			{
				server->CreateChannel(tokens[1]);
				channel = server->GetChannel(tokens[1]);
				channel->AddMember(client);
				channel->AddOperator(client);
				send(client->GetSocket(), message.c_str(), message.size(),  MSG_EOR);
				client->SetCurrentChannel(tokens[1]);
				client->inChannel = true;
			}
			else
			{
				channel->AddMember(client);
				client->SetCurrentChannel(tokens[1]);
				send(client->GetSocket(), message.c_str(), message.size(),  MSG_EOR);
				client->inChannel = true;
			}
		}
	}
}


// void SendMsg(Client *client, const std::string &message) {
//     if (!client) {
//         std::cerr << "Error: Null client pointer in SendMsg." << std::endl;
//         return;
//     }

//     std::string msgToSend = message + "\r\n"; // Append IRC standard line ending
//     int socket = client->GetSocket();         // Get the client's socket

//     if (send(socket, msgToSend.c_str(), msgToSend.length(), 0) == -1) {
//         std::cerr << "Error: Failed to send message to client " << client->getNickname() << std::endl;
//     }
// }

// void JoinChannel(std::vector<std::string> tokens, Server *server, Client *client) {
//     if (tokens.size() < 2) {
//         SendErrorMsg("461", "JOIN :Not enough parameters", client);
//         return;
//     }

//     std::string channelName = tokens[1];
//     if (channelName[0] != '#') { // Channels typically start with '#'
//         SendErrorMsg("403", "JOIN :Invalid channel name", client);
//         return;
//     }

//     // Check if the channel exists, otherwise create it
//     Channel *channel = server->GetChannel(channelName);
//     if (!channel) {
//         server->CreateChannel(channelName);
//         channel = server->GetChannel(channelName);
//         std::cout << "Channel " << channelName << " created." << std::endl;
//     }

//     // Check if the client is already a member
//     if (channel->HasMember(client->getNickname())) {
//         SendErrorMsg("443", channelName + " :You're already in this channel", client);
//         return;
//     }

//     // Add the client to the channel
//     channel->AddMember(client);
//     client->SetCurrentChannel(channelName); // Update the client's current channel
//     std::cout << "Client " << client->getNickname() << " joined channel " << channelName << "." << std::endl;

//     // Send confirmation to the client
//     SendMsg(client, "You have joined channel " + channelName);

//     // Broadcast the join message to all channel members
//     std::string joinMessage = client->getNickname() + " has joined the channel.";
//     channel->broadcast(joinMessage, client);
// }

