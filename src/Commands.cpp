#include "../includes/Commands.hpp"

bool	CheckClientPass(Client *client)
{
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

void	CheckPass(std::string buffer, int index, Client *client, Server server)
{
	std::string password;

	std::cout << "checking password" << std::endl;
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
