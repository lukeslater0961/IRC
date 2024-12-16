#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"
#include <sys/socket.h>
#include <sstream>

std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        if (!token.empty() && token[token.size() - 1] == '\n')
            token.erase(token.size() - 1);
        result.push_back(token);
    }

    return result;
}

void	SendErrorMsg(std::string prefix, std::string errorType, Client *client)
{
	std::string errormsg = prefix + ':' + errorType;
	send(client->GetSocket(), errormsg.c_str(), errormsg.size(), MSG_EOR);
}

int	ErrorMngment(std::string msg)
{
	std::cout << msg << std::endl;
	return (1);
}

void	BroadcastToChannel(std::vector<std::string> tokens, Client *client, Server *server)
{
	Channel *channel = server->GetChannel(client->GetCurrentChannel());
	std::map<std::string, Client *> members = channel->GetMembers();
	std::string msg = '\r' + client->getNickname() + ": ";

	for(size_t i = 0; i < tokens.size(); i++)
		msg += tokens[i];

	msg += '\n';
    for (std::map<std::string, Client *>::iterator it = members.begin(); it != members.end(); ++it)
    {
        if (it->second != client)
			send(it->second->GetSocket(), msg.c_str(), msg.size(), MSG_EOR);
    }
}

void LoginCommands(std::string buffer, Client *client, Server *server)
{
	size_t j = 0;
    std::string token;
    std::string commands[] = {"PASS", "NICK", "USER"};

    if (buffer.empty())
        return;

    std::vector<std::string> tokens = split(buffer, ' ');
    for (; j < sizeof(commands) / sizeof(commands[0]); j++) {
        if (!std::strcmp(tokens[0].c_str(), commands[j].c_str()))
            break;
    }
    switch (j) {
        case 0:
            CheckPass(tokens, client, server);
            break;
        case 1:
            CheckNickname(tokens, client, server);
            break;
        case 2:
			SetUsername(tokens, client);
            break;
        default:
			SendErrorMsg("No channel joined. " , "Try /join #<channel>\n", client);
            break;
    }
}

void	DoCommands(std::string buffer, Client *client, Server *server)
{
	std::string	commands[] = {"PASS", "NICK", "USER", "JOIN"};
    std::vector<std::string> tokens = split(buffer, ' ');
	size_t i = 0;

    if (tokens.empty())
		return;

	for(; i < sizeof(commands)/sizeof(commands[0]); i++)
	{
		if (!std::strcmp(tokens[0].c_str(), commands[i].c_str()))
			break;
	}

	switch (i) {
        case 0:
            CheckPass(tokens, client, server);
            break;
        case 1:
            CheckNickname(tokens, client, server);
            break;
        case 2:
			SetUsername(tokens, client);
            break;
		case 3:
			JoinChannel(tokens, server, client);
			break;
		default:
			if (!client->inChannel)
				SendErrorMsg("No channel joined. " , "Try /join #<channel>\n", client);
			else
				BroadcastToChannel(tokens, client, server);

	}
    // std::string command = tokens[0];

    // if (command == "KICK") {
    //     if (tokens.size() < 3) {
    //         SendErrorMsg("461", "KICK :Not enough parameters", client);
    //         return;
    //     }
    //     std::string channel = tokens[1];
    //     std::string user = tokens[2];
    //     KickCommand(server, channel, user, client);
    // } else if (command == "MODE") {
    //     if (tokens.size() < 3) {
    //         SendErrorMsg("461", "MODE :Not enough parameters", client);
    //         return;
    //     }

    //     std::string target = tokens[1];
    //     std::string mode = tokens[2];
    //     std::string param = (tokens.size() > 3) ? tokens[3] : "";

    //     // Call the ModeCommand handler with the parsed arguments
    //     ModeCommand(server, client, target, mode, param);
    // } else if (command == "TOPIC") {
    //     if (tokens.size() < 2) {
    //         SendErrorMsg("461", "TOPIC :Not enough parameters", client);
    //         return;
    //     }
    //     std::string channel = tokens[1];
    //     std::string topic = tokens.size() > 2 ? tokens[2] : "";
    //     TopicCommand(server, channel, topic, client);
    // } else if (command == "INVITE") {
    //     if (tokens.size() < 3) {
    //         SendErrorMsg("461", "INVITE :Not enough parameters", client);
    //         return;
    //     }
    //     std::string user = tokens[1];
    //     std::string channel = tokens[2];
    //     InviteCommand(server, user, channel, client);
    // } else {
    //     SendErrorMsg("421 " + command, "Unknown command", client);
    // }
}

void	ParseMessage(std::string buffer, Server *server, int clientSocket)
{
	Client *client = server->FindClient(clientSocket);
    if (!client){
        std::cerr << "Client not found for socket: " << clientSocket << std::endl;
        return;
    }
	if (!client->GetPassword() || client->getNickname().empty() || client->getUsername().empty())
		LoginCommands(buffer, client, server);
	else 
		DoCommands(buffer, client, server);
} 
