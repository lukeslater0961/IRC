#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"

class Server{
	public:
		Server();
		void	SetPort(int portValue);
		void	SetPassword(std::string password);

		std::string GetPassword();
		int GetPort();
	private:
		std::string _password;
		int			_port;
};

int	SetupServer(char **argv);
void	StartServer(Server server);
#endif

