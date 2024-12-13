#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"



int	ErrorMngment(std::string msg)
{
	std::cout << msg << std::endl;
	return (1);
}



void LoginCommands(std::string buffer, Client *client, Server server)
{
    size_t i = 0;
    std::string token;
    std::string commands[] = {"PASS", "NICK", "USER"};

    if (buffer.empty())
        return;

    // Extract the first word (token) from the buffer
    while (i < buffer.size() && buffer[i] != ' ')
        token += buffer[i++];
    // Find the command index
    size_t j = 0;
    for (; j < sizeof(commands) / sizeof(commands[0]); j++) {
        if (token == commands[j]) {
            break;
        }
    }

    // Handle commands based on index
    switch (j) {
        case 0: // PASS
            std::cout << "checking password" << std::endl;
            CheckPass(buffer, i, client, server);
            break;
        case 1: // NICK
            std::cout << "setting nick" << std::endl;
			i++;
            CheckNickname(&buffer[i], client, &server);
            break;
        case 2: // USER
            std::cout << "processing user command" << std::endl;
            // Add USER command handling here if needed
            break;
        default:
            std::cerr << "Unknown command: " << token << std::endl;
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
