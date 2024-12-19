#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Client.hpp"
#include "Server.hpp"
#include "Utils.hpp"

void	CheckPass(std::vector<std::string> tokens, Client *client, Server *server);
void    CheckNickname(std::vector<std::string> tokens, Client *client, Server *server);
void	SetUsername(std::vector<std::string> tokens, Client *client);
void    KickCommand(Server &server, const std::string &channelName, Client *operatorClient, const std::string &targetNickname);
void    InviteCommand(Server &server, const std::string &channelName, Client *operatorClient, std::string target);
void    TopicCommand(Server &server, const std::string &channelName, Client *operatorClient, std::vector<std::string> &tokens);
void ModeCommand(Server &server, Client *operatorClient, std::vector<std::string> tokens);
void	JoinChannel(std::vector<std::string> tokens, Server *server, Client *client);
void	Broadcast(std::vector<std::string> tokens, Server *server, Client *client);

#endif
