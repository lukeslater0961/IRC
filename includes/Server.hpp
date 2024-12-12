#ifndef SERVER_HPP
#define SERVER_HPP

#include "Utils.hpp"

class Server{
	public:
		Server();
		void	SetPort(int portValue);
		void	SetPassword(std::string password);

		std::string GetPassword();
		int			GetPort();
		int	SetupServer(char **argv);

	private:
		std::string _password;
		bool 		_goodPassword;
		int			_port;
};

int SetupServer(char **argv);
void	StartServer(Server server);
#endif

