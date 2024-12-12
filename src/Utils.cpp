#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"
#include <cstring>

int	ErrorMngment(std::string msg)
{
	std::cout << msg << std::endl;
	return (1);
}

void	CheckPass(std::string buffer, int index, Server server)
{
	std::string password;

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
			std::cout << "password is correct" << std::endl;
		else
			std::cout << "password is incorrect" << std::endl;
			//set password true in client
	}

}

void	ParseMessage(std::string buffer, Server server)
{
	int	i = -1;
	size_t j = 0;
	std::string token;
	std::string commands[] = {"PASS", "NICK"};

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
			CheckPass(buffer, i, server);
			break;
		case 2:
			std::cout << "setting nick" << std::endl;
			//SetNick(buffer);
			break;
		default:
			break;
	}
} 
