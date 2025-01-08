#include "../../includes/Commands.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/Utils.hpp"
#include "../../includes/Client.hpp"
#include "../../includes/Channel.hpp"
#include <sys/socket.h>
#include <string>

int	CheckModeType(std::string tokens)
{
	if (tokens[0] == '-')
		return (1);
	else if (tokens[0] == '+')
		return (2);
	else
		return (0);
}


int ParseValues(const std::vector<std::string>& tokens, std::string& modes, std::vector<std::string>& arguments)
{
    if (tokens.size() < 3) {
        std::cerr << "Invalid MODE command: Not enough parameters." << std::endl;
        return 1;
    }

    std::string modeString = tokens[2];
    int modeType = CheckModeType(modeString);
    if (modeType == 1 || modeType == 2) {
        modeString = modeString.substr(1); // Remove + or -
    }

	for (std::string::iterator it = modeString.begin(); it != modeString.end(); ++it) {
		char mode = *it;
		if (modes.find(mode) == std::string::npos) {
            std::cerr << "Invalid mode: " << mode << std::endl;
            return 1; // Error
        }
    }

    if (tokens.size() > 3)
        arguments.assign(tokens.begin() + 3, tokens.end());

    return 0;
}

void CheckAndExecMode(Channel& channel, const std::string& mode, const std::string& argument, bool addMode)
{
    if (mode == "i")
        channel.setInviteOnly(addMode);
    else if (mode == "t")
        channel.setTopicRestriction(addMode);
    else if (mode == "k") {
        if (addMode)
            channel.SetKey(argument);
        else
            channel.clearKey();
    } else if (mode == "o") {
        if (addMode)
            channel.AddOperator(argument);
        else
            channel.RemoveOperator(argument);
    } else if (mode == "l") {
        if (addMode) {
            int limit = std::atoi(argument.c_str());
            channel.setUserLimit(limit);
        } else
            channel.clearUserLimit();
    }
}

void SendModeResponse(Client* client, const std::string& response)
{
    if (send(client->GetSocket(), response.c_str(), response.length(), 0) == -1)
        std::cerr << "Error: Failed to send MODE response to client " << client->getNickname() << std::endl;
}


void ModeCommand(Server &server, Client *operatorClient, std::vector<std::string> tokens)
{
    std::string errorMessage;

    if (tokens.size() < 3)
        return;

    Channel *channel = server.GetChannel(tokens[1]);
    if (!channel) {
        SendModeResponse(operatorClient, ":server 403 " + tokens[1] + " :No such channel\r\n");
        return;
    }

    if (!channel->HasOperator(operatorClient->getNickname())) {
        errorMessage = ":localhost 482 " + operatorClient->getNickname() + " " + tokens[1] + " :You're not channel operator\r\n";
        SendMsg(operatorClient, errorMessage);
        return;
    }

    std::string modeString = tokens[2];
    std::vector<std::string> arguments;
    for (size_t i = 3; i < tokens.size(); ++i)
        arguments.push_back(tokens[i]);

    bool addMode = true;
    size_t argIndex = 0;

    for (size_t i = 0; i < modeString.size(); ++i) {
        char mode = modeString[i];

        if (mode == '+') {
            addMode = true;
            continue;
        } else if (mode == '-') {
            addMode = false;
            continue;
        }

        if (mode == 'i')
            channel->setInviteOnly(addMode);
        else if (mode == 't')
            channel->setTopicRestriction(addMode);
        else if (mode == 'k') {
            if (addMode) {
                if (argIndex >= arguments.size()) {
					std::string errorMessage = ":localhost 696 " + operatorClient->getNickname() +
                                " " + channel->getName() + " k * :You must specify a parameter for the key mode. Syntax: <key>.";
					SendMsg(operatorClient, errorMessage);
                    return;
                }
                channel->SetKey(arguments[argIndex]);
                ++argIndex;
            } else
                channel->clearKey();
        } else if (mode == 'o') {
            if (argIndex >= arguments.size()) {
               	std::string errorMessage = ":localhost 696 " + operatorClient->getNickname() +
                                " " + channel->getName() + " o * :You must specify a parameter for the key mode. Syntax: <nick>.";
					SendMsg(operatorClient, errorMessage);
                return;
            }
            if (addMode)
                channel->AddOperator(arguments[argIndex]);
            else
                channel->RemoveOperator(arguments[argIndex]);
            ++argIndex;
        } else if (mode == 'l') {
            if (addMode) {
                if (argIndex >= arguments.size()) {
                    std::string errorMessage = ":localhost 696 " + operatorClient->getNickname() +
                                " " + channel->getName() + " l * :You must specify a parameter for the key mode. Syntax: <limit>.";
					SendMsg(operatorClient, errorMessage);                    return;
                }
                int limit = std::atoi(arguments[argIndex].c_str());
                channel->setUserLimit(limit);
                ++argIndex;
            } else
                channel->clearUserLimit();
        } else
		{
			SendModeResponse(operatorClient, ":server 472 " + std::string(1, mode) + " :Invalid mode\r\n");
			return;
		}
    }

    std::string response = ":server MODE " + tokens[1] + " " + tokens[2];
    for (size_t i = 3; i < tokens.size(); ++i)
        response += " " + tokens[i];
    response += "\r\n";

    SendModeResponse(operatorClient, response);
}



