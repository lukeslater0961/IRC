#include "../includes/Utils.hpp"
#include <cstring>

int	ErrorMngment(std::string msg)
{
	std::cout << msg << std::endl;
	return (1);
}

void	ParseMessage(std::string buffer)
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
			//CheckPass(buffer);
			break;
		case 2:
			std::cout << "setting nick" << std::endl;
			//SetNick(buffer);
			break;
		default:
			break;
	}
} 
