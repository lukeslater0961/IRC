#include "../includes/Utils.hpp"
#include "../includes/Server.hpp"
#include "../includes/Commands.hpp"
#include <sys/socket.h>
#include <sstream>

std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        if (!token.empty() && token[token.size() - 1] == '\n')
            token.erase(token.size() - 1);
        result.push_back(token);
    }

    return result;
}

void	SendErrorMsg(std::string prefix, std::string errorType, Client *client)
{
	std::string errormsg = prefix + ':' + errorType;
	std::cout << client->GetSocket() << std::endl;
	send(client->GetSocket(), errormsg.c_str(), errormsg.size(), MSG_EOR);
}

int	ErrorMngment(std::string msg)
{
	std::cout << msg << std::endl;
	return (1);
}

void	BroadcastToChannel(std::vector<std::string> tokens, Client *client, Server *server)
{
	Channel *channel = server->GetChannel(client->GetCurrentChannel());
	std::map<std::string, Client *> members = channel->GetMembers();
    std::string msg;

	for(size_t i = 0; i < tokens.size(); i++)
    {
        if (i > 0)
            msg += " ";
        msg += tokens[i];
    }

	msg += '\n';
    for (std::map<std::string, Client *>::iterator it = members.begin(); it != members.end(); ++it)
    {
        // if (it->second != client)
			send(it->second->GetSocket(), msg.c_str(), msg.size(), MSG_EOR);
    }
}

void DoCommands(std::string buffer, Client *client, Server *server)
{
	size_t j;
    size_t i;
    std::string token;
    std::string unloggedcommands[] = {"PASS", "NICK", "USER", "QUIT"};
	std::string	commandslogged[] = {"JOIN", "PRIVMSG", "WHO", "MODE", "TOPIC", "KICK", "INVITE", "CAP"};

    if (buffer.empty())
        return;

    std::vector<std::string> newbuffer = split(buffer, '\n');
	for (std::vector<std::string>::iterator it = newbuffer.begin(); it != newbuffer.end(); it++)
	{
        j = 0;
        i = 0;
		std::string command_line = it->data();
		command_line = command_line.substr(0, command_line.find('\r'));
		std::vector<std::string> tokens = split(command_line, ' ');
		std::string tokens2 = command_line.substr(0, command_line.find(' '));
		
		for (; j < sizeof(unloggedcommands) / sizeof(unloggedcommands[0]); j++)
		{
			if (!std::strcmp(tokens2.c_str(), unloggedcommands[j].c_str()))
				break;
		}
		std::cout << j << " ->" << tokens[0] << std::endl;
        if ((client->GetPassword() && client->getNickname().size() > 1 && client->getUsername().size() > 1) && j == sizeof(unloggedcommands) / sizeof(unloggedcommands[0]))
        {
            for (; i < sizeof(commandslogged) / sizeof(commandslogged[0]); i++)
            {
                if (!std::strcmp(tokens2.c_str(), commandslogged[i].c_str()))
                    break;
            }
        }
        // if (j == sizeof(unloggedcommands) / sizeof(unloggedcommands[0]))
        j += i;
		std::cout << j << " ->" << tokens[0] << std::endl;
		switch (j) {
			case 0:
				CheckPass(tokens, client, server);
				break;
			case 1:
				CheckNickname(tokens, client, server);
				break;
			case 2:
				SetUsername(tokens, client);
				break;
            case 3:
                client->StopClient();
                break;
            case 4:
	    		JoinChannel(tokens, server, client);
                break;
            case 5:
                Broadcast(tokens, server, client);
                break;
            case 6:
                break;
            case 7:
                // ModeCommand(*server, client, tokens[1], tokens[2], tokens[3]);
                break;
            case 8:
                std::cout << "TOPIC" << std::endl;
                TopicCommand(*server, tokens[1], client, tokens);
                break;
            case 9:
                KickCommand(*server, tokens[1], client, tokens[2]);
                break;
            case 10:
                InviteCommand(*server, tokens[1], client, tokens[2]);
                break;
            case 11:
                break;
			default:
				SendErrorMsg("No channel joined. " , "Try /join #<channel>\n", client);
				break;
		}
	}
}



void	ParseMessage(std::string buffer, Server *server, int clientSocket)
{
	Client *client = server->FindClient(clientSocket);
    if (!client){
        std::cerr << "Client not found for socket: " << clientSocket << std::endl;
        return;
    }
	std::cout << "receiving ::: " + buffer << std::endl;
    DoCommands(buffer, client, server);
}
