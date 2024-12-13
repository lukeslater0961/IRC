#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "Client.hpp"
#include "Server.hpp"
#include "Utils.hpp"

void	CheckPass(std::string buffer, int index, Client *client, Server server);
void    CheckNickname(std::string buffer, Client *client, Server *server);
#endif
