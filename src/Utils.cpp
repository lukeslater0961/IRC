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
	std::string errormsg = prefix + ':' + errorType + "\r\n";
	std::cout << client->GetSocket() << std::endl;
	send(client->GetSocket(), errormsg.c_str(), errormsg.size(), 0);
}

int	ErrorMngment(std::string msg)
{
	std::cout << msg << std::endl;
	return (1);
}

void	BroadcastToUser(std::vector<std::string> tokens, Server *server, Client *client)
{
	if (tokens.size() == 1)
		return ;
	else if (tokens.size() == 3)
	{
        for (std::vector<Client *>::iterator it = server->client.begin(); it != server->client.end(); it++)
        {
            if ((*it)->getNickname() == tokens[1] && (*it)->getNickname() != client->getNickname())
            {
                std::string msg = ":" + client->getNickname() + " PRIVMSG " + tokens[1] + " " + tokens[2];
                SendMsg(*it, msg);
            }
        }
	}
}

void BroadcastToChannel(std::vector<std::string> tokens, Client *client, Server *server)
{
    Channel *channel = server->GetChannel(client->GetCurrentChannel());

	if (tokens[1][0] != '#')
		BroadcastToUser(tokens, server, client);
	else
	{
        if (!channel) {
            std::cerr << "Error: Client is not in a valid channel." << std::endl;
            return;
        }
        std::map<std::string, Client *> members = channel->GetMembers();
        if (members.empty()) {
            std::cerr << "Error: Channel has no members." << std::endl;
            return;
        }
		std::string msg = ":" + client->getNickname() + " PRIVMSG " + tokens[1] + " ";

		for (size_t i = 2; i < tokens.size(); ++i) { 
			if (i > 2)
				msg += " ";
			msg += tokens[i];
		}
		msg += "\r\n"; // Append IRC-compliant line ending

		for (std::map<std::string, Client *>::iterator it = members.begin(); it != members.end(); ++it) {
			if (it->second == client)
				continue; // Skip sending the message to the sender

			if (send(it->second->GetSocket(), msg.c_str(), msg.size(), 0) == -1)
				std::cerr << "Error: Failed to send message to " << it->first << std::endl;
		}
		// Debug: Log the final broadcasted message
		std::cout << "Broadcasted message (excluding sender): " << msg << std::endl;
	}
}

void Quit(std::string channelName, Client *client, Server *server)
{
    Channel *channel = server->GetChannel(channelName);
    if (channel->HasMember(client->getNickname()))
        channel->RemoveMember(client->getNickname());
    std::string msg = ":" + client->getNickname() + " QUIT\n";
    channel->broadcast(msg, NULL);
}

void DoCommands(std::string buffer, Client *client, Server *server)
{
	size_t j;
    size_t i;
    std::string token;
    std::string unloggedcommands[] = {"PASS", "NICK", "USER", "QUIT", "CAP"};
	std::string	commandslogged[] = {"JOIN", "PRIVMSG", "WHO", "MODE", "TOPIC", "KICK", "INVITE"};

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
        if (j == sizeof(unloggedcommands) / sizeof(unloggedcommands[0]))
        {
            if (!client->GetPassword() || client->getNickname().size() < 1 || client->getUsername().size() < 1)
            {
                SendErrorMsg("451", "You have not registered", client);
                return;
            }
            for (; i < sizeof(commandslogged) / sizeof(commandslogged[0]); i++)
            {
                if (!std::strcmp(tokens2.c_str(), commandslogged[i].c_str()))
                    break;
            }
        }

        j += i;
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
                Quit(client->GetCurrentChannel(), client, server);
                break;
            case 4:
                if (tokens.size() > 1 && tokens[1] == "LS") {
                    SendMsg(client, ":localhost CAP * LS :"); // Indicate no capabilities supported
                } 
                break;
            case 5:
	    		JoinChannel(tokens, server, client);
                break;
            case 6:
                BroadcastToChannel(tokens, client, server);
                break;
            case 7:
                break;
            case 8:
                ModeCommand(*server, client, tokens);
                break;
            case 9:
                TopicCommand(*server, tokens[1], client, tokens);
                break;
            case 10:
                KickCommand(*server, tokens[1], client, tokens[2]);
                break;
            case 11:
				if (tokens.size() != 3)
				{
					SendErrorMsg("461", "INVITE :Not enough parameters", client);
					break;
				}
                InviteCommand(*server, tokens[2], client, tokens[1]);
                break;
            default:
                if (!client->inChannel)
                {
                    std::string errMessage = ":localhost 403 " + client->getNickname() + " :No channel joined. Try /join #<channel>";
                    SendMsg(client, errMessage);
                    }

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
    DoCommands(buffer, client, server);
}
