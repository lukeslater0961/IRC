#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Client.hpp"
#include "Server.hpp"
#include "Utils.hpp"

void	CheckPass(std::string buffer, int index, Client *client, Server *server);
void    CheckNickname(std::string buffer, Client *client, Server *server);
void	SetUsername(std::string buffer, Client *client);
void    KickCommand(Server &server, const std::string &channelName, Client *operatorClient, const std::string &targetNickname);
void    InviteCommand(Server &server, const std::string &channelName, Client *operatorClient, Client *targetClient);
void    TopicCommand(Server &server, const std::string &channelName, Client *operatorClient, const std::string &newTopic);
void    ModeCommand(Server &server, Client *operatorClient, const std::string &channelName, const std::string &mode, const std::string &param = "");

#endif
