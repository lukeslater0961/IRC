#include "../../includes/Commands.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Utils.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"
#include <sys/socket.h>

int	CheckModeType(std::string tokens)
{
	if (tokens[0] == '-')
		return (1);
	else if (tokens[0] == '+')
		return (2);
	else
		return (0);
}

int	ParseValues(std::vector<std::string> tokens)
{
	std::string mode = "itkol";
	std::string args;

	if (tokens.size() == 1)
		return (1);
	else
	{
		if (tokens.size() == 2 && tokens[1][0] == '#')
			return (1);
		else		
		{
			int modeType = CheckModeType(tokens[2]);
			if (modeType == 1 || modeType == 2)
				tokens[2] = tokens[2].substr(1);		
			if ((modeType == 2 || modeType == 0) && (tokens[2].size() * 2) != tokens.size() - 3 + tokens[2].size() )
			{
				std::cout <<"size isnt correct" << std::endl;
				return (1);
			}
			for (size_t j = 0; j < tokens[2].size();j++)
			{
				if (mode.find(tokens[2][j]) == std::string::npos)
				{
					std::cout << "cant find char" <<std::endl;
					return (1);
				}
			}
			if (modeType == 1)
				std::cout << "removing mode" << std::endl;
		}
	}
	return (0);
}

void	CheckAndExecMode(int modeIndex, int argsIndex, std::vector<std::string> tokens)
{
	(void)tokens;
	(void)argsIndex;
	switch (modeIndex) {
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
	}
}

void ModeCommand(Server &server, Client *operatorClient, std::vector<std::string> tokens)
{
	std::string modes = "itkol";
	(void)operatorClient;
	(void)server;

	if (ParseValues(tokens))
			return ;
	std::string modeArgs = tokens[2];
	//need to send all the individual messages to the client
	//parsing works normally (checks for the amount of modes and arguments (1 arg for each mode) and we also now woork with + and - (for the - we just need to send the correct messages aswell))
}

