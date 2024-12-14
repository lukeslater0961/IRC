#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include <sys/socket.h>

void	SetUsername(std::string buffer, Client *client)
{
	std::vector<std::string> words = split(buffer, ' ');

	if (!buffer[0])
		SendErrorMsg("USER :", USER_USAGE, client);
	else
		client->setUsername(words[0]);
}

bool	CheckClientPass(Client *client)
{
	if (client)
	{
		if (client->GetPassword())
		{
			SendErrorMsg("462 ", ALR_REGISTERED, client);
			return (true);
		}
	}
	return (false);
}

void	CheckPass(std::string buffer, int index, Client *client, Server *server)
{
	std::string password;

	if (!CheckClientPass(client))
	{
		while (buffer[++index] != ' ' && buffer[index] != '\0' && buffer[index] != '\n')
			password += buffer[index];

		password[index] = '\0';
		if (buffer[index] != '\n' && buffer[index] != '\0')
		{
			SendErrorMsg("464 PASS", PASS_INC, client);
			return;
		}
		else
		{
			if (!std::strcmp(password.c_str(), server->GetPassword().c_str()))
				client->SetPassword(true);
			else if (buffer[index] == '\0')
			{
				SendErrorMsg("461 PASS", PASS_INC_PARAM, client);
				send(client->GetSocket(), PASS_USAGE, sizeof(PASS_USAGE), MSG_EOR);
			}
			else 
				SendErrorMsg("464 PASS", PASS_INC, client);
		}
	}
}

void CheckNickname(std::string buffer, Client *client, Server *server)
{
	std::string invChars[] = {":", ",", "*", "@", "!" , "."};
	std::string nick;
	int i = -1;

	if (!buffer[0])
		SendErrorMsg("Usage: ", NICK_USAGE, client);

	while (buffer[++i] && buffer[i] != '\n' && buffer[i] != ' ')
		nick += buffer[i];

	for (size_t i = 0; i < sizeof(invChars)/sizeof(invChars[0]); ++i)
	{
		if (!buffer.find(invChars[i]))		
		{
			SendErrorMsg("432 " + nick, NON_NICK, client);
			return;
		}
	}

	for (std::vector<Client *>::iterator it = server->client.begin(); it != server->client.end(); it++)
	{
		if ((*it)->getNickname() == nick)
		{
			SendErrorMsg("433 " + nick, NICK_USE, client);
			return;
		}
	}
	client->setNickname(nick);
}
