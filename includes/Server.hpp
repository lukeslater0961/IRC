#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include <vector>
#include <csignal>


class Server{
	public:
		Server();
		~Server();
		void		SetPort(int portValue){this->_port = portValue;};
		void		SetPassword(std::string password){this->_password = password;};

		int			GetPort(){return this->_port;}
		std::string GetPassword(){return this->_password;};

		void		AddClient(int clientSocket);
		Client*		FindClient(int clientSocket);

		void		CreateChannel(const std::string &name);
		void		DeleteChannel(const std::string &name);
		Channel*	GetChannel(const std::string &name);

		std::vector<Client *> client;
	private:
		std::string						_password;
		std::map<std::string, Channel*>	_channels;
		int								_port;
};

int		SetupServer(char **argv);
void	StartServer(Server &server);
void	ParseMessage(std::string buffer, Server *server, int clientSocket);
void	DeleteClient(int clientSocket, Server &server);

void SetupPoll(struct pollfd* fds);
bool PollSockets(struct pollfd* fds, int nfds);
void HandleNewConnection(struct pollfd* fds, int& nfds, Server& server);
void ProcessClientSockets(struct pollfd* fds, int nfds, Server& server, char* buffer);
void HandleClientMessage(int clientSocket, Server& server, char* buffer, int bytesReceived);
void HandleClientDisconnection(struct pollfd* fds, int index, int clientSocket, Server& server, std::string channelName);
void InitializeServer(Server& server);
#endif

