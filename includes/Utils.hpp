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
#include <vector>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 100

  

int		ErrorMngment(std::string msg);
void	signalHandler(int signal);
void	SendErrorMsg(std::string prefix, std::string errorType, Client *client);
std::vector<std::string> split(const std::string& str, char delimiter);



# define INVALID_ARGS "usage: ./ircserv <port> <password>\n"
# define UNKNOWN_CMD " :Unknown command\n"
# define ALR_REGISTERED "You may not reregister\n"

# define PASS_INC " :Password is incorrect\n"
# define PASS_INC_PARAM " :Not enough parameters\n"
# define PASS_USAGE "PASS :<password>\n"

# define NICK_EXISTS "ERR_NICKCOLLISION\n"
# define NON_NICK " Erroneous Nickname\n"
# define NICK_USAGE "NICK <nickname>, sets your nick\n"
# define NICK_USE ":Nickname already in use\n"

# define USER_USAGE "<username> <realname>\n"

#endif
