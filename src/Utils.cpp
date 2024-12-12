#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"

int	ErrorMngment(std::string msg)
{
	std::cout << msg << std::endl;
	return (1);
}

void	LoginCommands(std::string buffer, Client *client, Server server)
{
	int	i = -1;
	size_t j = 0;
	std::string token;
	std::string commands[] = {"PASS", "NICK", "USER"};

	if (buffer.empty())
		return ;
	while (buffer[++i] != ' ' && buffer[i] != '\0')
		token += buffer[i];
	
	for (j = 0; j < sizeof(token)/sizeof(token[0]); j++)
		if (std::strcmp(token.c_str(), commands[j].c_str()))
			break ;

	switch (j) {
		case 1:
			std::cout << "checking password" << std::endl;
			CheckPass(buffer, i, client, server);
			break;
		case 2:
			std::cout << "setting nick" << std::endl;
			CheckNickname(buffer);
			break;
		default:
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
