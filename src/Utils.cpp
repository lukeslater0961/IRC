#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"
#include <cstring>

int	ErrorMngment(std::string msg)
{
	std::cout << msg << std::endl;
	return (1);
}



bool	CheckClientPass(int clientSocket, Client *client)
{
	(void)clientSocket;
	if (client)
	{
		if (client->GetPassword())
		{
			//send message to client saying (password already set)
			std::cout << "password already set" << std::endl;
			return (true);
		}
	}
	return (false);
}

void	CheckPass(std::string buffer, int index, Server server, int clientSocket)
{
	std::string password;
	Client *client = server.FindClient(clientSocket);

	std::cout << &client << std::endl;
	if (!CheckClientPass(clientSocket, client))
	{
		while (buffer[++index] != ' ' && buffer[index] != '\0' && buffer[index] != '\n')
			password += buffer[index];

		password[index] = '\0';
		std::cout << static_cast<int>(buffer[index]) << std::endl;
		if (buffer[index] != '\n' && buffer[index] != '\0')
		{
			std::cout << "password is incorrect" << std::endl;
			return;
		}
		else
		{
			std::cout << server.GetPassword() << " " << password << std::endl;
			if (!std::strcmp(password.c_str(), server.GetPassword().c_str()))
			{
				std::cout << "password is correct" << std::endl;
				// client->SetPassword(true);
			}
			else
				std::cout << "password is incorrect" << std::endl;
				//set password true in client
		}
	}
}

void	ParseMessage(std::string buffer, Server server, int clientSocket)
{
	int	i = -1;
	size_t j = 0;
	std::string token;
	std::string commands[] = {"PASS", "NICK"};
	(void)server;
	(void)clientSocket;
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
			CheckPass(buffer, i, server, clientSocket);
			break;
		case 2:
			std::cout << "setting nick" << std::endl;
			// setNickname(buffer);
			break;
		default:
			break;
	}
} 
