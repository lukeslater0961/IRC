#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"
#include "Client.hpp"
#include <vector>

class Server{
	public:
		Server();
		void	SetPort(int portValue){this->_port = portValue;};
		void	SetPassword(std::string password){this->_password = password;};

		int 	GetPort(){return this->_port;}
		std::string GetPassword(){return this->_password;};

		void AddClient(int clientSocket);
		Client* FindClient(int clientSocket);

	private:
		std::string _password;
		std::vector<Client *> client;
		int			_port;
};

int SetupServer(char **argv);
void	StartServer(Server server);
void	ParseMessage(std::string buffer, Server server, int clientSocket);
#endif

