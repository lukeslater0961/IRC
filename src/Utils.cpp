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

void LoginCommands(std::string buffer, Client *client, Server server)
{
    size_t i = 0;
	size_t j = 0;
    std::string token;
    std::string commands[] = {"PASS", "NICK", "USER"};

    if (buffer.empty())
        return;

    while (i < buffer.size() && buffer[i] != ' ' && buffer[i] != '\n')
        token += buffer[i++];
    for (; j < sizeof(commands) / sizeof(commands[0]); j++) {
        if (token == commands[j]) {
            break;
        }
    }

    switch (j) {
        case 0:
            CheckPass(buffer, i , client, &server);
            break;
        case 1:
            CheckNickname(&buffer[i + 1], client, &server);
            break;
        case 2:
            std::cout << "processing user command" << std::endl;
			SetUsername(&buffer[i + 1], client);
            break;
        default:
			SendErrorMsg("421 " + token, UNKNOWN_CMD, client);
            break;
    }
}


void	DoCommands(std::string buffer, Client *client, Server server)
{
	(void)server;
	(void)buffer;
	(void)client;
	std::cout << "doing normal comamnds"<< std::endl;
}

void	ParseMessage(std::string buffer, Server server, int clientSocket)
{
	Client *client = server.FindClient(clientSocket);
	if (!client->GetPassword() || client->getNickname().empty() || client->getUsername().empty())
		LoginCommands(buffer, client, server);
	else 
		DoCommands(buffer, client, server);
} 
