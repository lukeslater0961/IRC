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
