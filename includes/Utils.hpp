#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <poll.h>
#include <cstring>
#include "Client.hpp"

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 100

  

int		ErrorMngment(std::string msg);
void	signalHandler(int signal);
void	SendErrorMsg(std::string prefix, std::string errorType, Client *client);

# define INVALID_ARGS "usage: ./ircserv <port> <password>"
# define PASS_SET "ERR_ALREADYREGISTRED\n"
# define NICK_EXISTS "ERR_NICKCOLLISION\n"
# define NON_NICK "ERR_NONICKNAMEGIVEN\n"

#endif
