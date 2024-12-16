#include "../includes/Commands.hpp"
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include <sys/socket.h>

void	JoinChannel(std::vector<std::string> tokens, Server *server, Client *client)
{
	if (tokens.size() == 1 || tokens.size() > 2)
		SendErrorMsg("Usage: ", JOIN_USAGE, client);
	else
	{
		if (!(tokens[1][0] == '#'))
			SendErrorMsg(tokens[1], CHANNEL_NAME, client);
		else
		{
			std::string message = JOINED + tokens[1] + "\n";
			Channel *channel = server->GetChannel(tokens[1]);
			if (!channel)
			{
				server->CreateChannel(tokens[1]);
				channel = server->GetChannel(tokens[1]);
				channel->AddMember(client);
				channel->AddOperator(client);
				send(client->GetSocket(), message.c_str(), message.size(),  MSG_EOR);
				client->SetCurrentChannel(tokens[1]);
				client->inChannel = true;
			}
			else
			{
				channel->AddMember(client);
				client->SetCurrentChannel(tokens[1]);
				send(client->GetSocket(), message.c_str(), message.size(),  MSG_EOR);
				client->inChannel = true;
			}
		}
	}
}
