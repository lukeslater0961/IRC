#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>

int	ErrorMngment(std::string msg);

# define INVALID_ARGS "usage: ./ircserv <port> <password>"
# define AUTHENTICATE "Welcome, to access the server please enter the password as follows: PASS <password>\n"

#endif
