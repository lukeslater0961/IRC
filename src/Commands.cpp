#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include <sys/socket.h>
#include <algorithm>

bool	CheckClientPass(Client *client)
{
	if (client)
	{
		if (client->GetPassword())
		{
			SendErrorMsg("PASS ", , client);
			std::cout << "password already set" << std::endl;
			return (true);
		}
	}
	return (false);
}

void	CheckPass(std::string buffer, int index, Client *client, Server server)
{
	std::string password;

	if (!CheckClientPass(client))
	{
		while (buffer[++index] != ' ' && buffer[index] != '\0' && buffer[index] != '\n')
			password += buffer[index];

		password[index] = '\0';
		if (buffer[index] != '\n' && buffer[index] != '\0')
		{
			std::cout << "password is incorrect" << std::endl;
			return;
		}
		else
		{
			if (!std::strcmp(password.c_str(), server.GetPassword().c_str()))
			{
				std::cout << "password is correct" << std::endl;
				client->SetPassword(true);
			}
			else
				std::cout << "password is incorrect" << std::endl;
		}
	}
}

void CheckNickname(std::string buffer, Client *client, Server *server)
{
	std::string invChars[] = {" ", ":", ",", "*", "@", "!", "."};

	for (size_t i = 0; i < sizeof(invChars)/sizeof(invChars[0]); ++i)
	{
		if (buffer.find(invChars[i]) != std::string::npos)		
		{
			send(client->GetSocket(), NON_NICK, sizeof(NON_NICK), MSG_EOR);
			return;
		}
	}

	for (std::vector<Client *>::iterator it = server->client.begin(); it != server->client.end(); it++)
	{
		if ((*it)->getNickname() == buffer)
		{
			//send message to client saying (nickname already taken)
			std::cout << "nickname already taken" << std::endl;
			return;
		}
	}
	client->setNickname(buffer);
	std::cout << "nickname set" << std::endl;
	
}
