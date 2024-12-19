#include "../../includes/Commands.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Utils.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"
#include <sys/socket.h>

bool	CheckModeType(std::string tokens)
{
	if (tokens[0] == '-')
		return (true);
	else 
		return (false);
}

int	ParseValues(std::vector<std::string> tokens, Server *server, Client *client)
{
	(void)client;
	(void)server;
	std::string msg;
	if (tokens.size() == 1)
		return (1);
	else
	{
		if (tokens.size() == 2 && tokens[1][0] == '#')
		{
			std::cout << tokens[1] << std::endl;
			Broadcast(split("Mode " + tokens[1] + " +nt", ' '), server, client);
			return (1);
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
	if (ParseValues(tokens, &server, operatorClient))
			return ;
	/*bool modeType = false;
	std::string modes[] = {"l", "k", "o"};
	std::string modeArgs = tokens[2];
	size_t i = 0;
	size_t j = 0;

	(void)server;
	(void)operatorClient;
	modeType = CheckModeType(tokens[2]);
	std::cout <<"hello"  << modeType << std::endl;
	if (modeType)
	{
		modeArgs.substr(1);
		for (; i < sizeof(modeArgs); i++)
		{
			for (;j < sizeof(modes)/sizeof(modes[0]); j++)
			{
				if (std::string(1, modeArgs[i]) == modes[j])
					break;
			}
			std::cout << tokens[3] <<std::endl;
			//CheckAndExecMode(j, i, split(tokens));
		}
	}*/
}
