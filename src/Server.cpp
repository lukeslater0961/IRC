#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include <cctype>
Server::Server(){}

void	Server::SetPort(int portValue){this->_port = portValue;}
void	Server::SetPassword(std::string password){this->_password = password;}

std::string Server::GetPassword(){return (this->_password);}
int Server::GetPort(){return (this->_port);}

int	SetupServer(char **argv)
{
	std::string port = argv[1];
	std::string password = argv[2];
	Server server;
	if (port.empty()||password.empty() || std::atoi(argv[1]) == 0)
		return(ErrorMngment(INVALID_ARGS));
	server.SetPort(std::atoi(argv[1]));
	server.SetPassword(argv[2]);
	std::cout << "Password :" << server.GetPassword() << std::endl << "Port:" << server.GetPort()<< std::endl;//to be removed
	StartServer(server);
	return(0);
}

void	StartServer(Server server)
{
	int	serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (serverSocket == -1)
	{
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	sockaddr_in server_adress; 
	server_adress.sin_family = AF_INET; 
	server_adress.sin_port = htons(server.GetPort()); 
	server_adress.sin_addr.s_addr = INADDR_ANY;
	
	if (bind(serverSocket, (struct sockaddr*)&server_adress, sizeof(server_adress)) < 0)
	{
		perror("Bind failed");
		close(serverSocket);
		exit(EXIT_FAILURE);
	}

    if (listen(serverSocket, SOMAXCONN) < 0) {
        perror("Listen failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << server.GetPort() << std::endl;
	//to put in another funtion
    while (true)
	{
        sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&client_address, &client_len);
        if (clientSocket < 0)
		{
            perror("Accept failed");
            continue;
        }
        std::cout << "New connection accepted.\n";

		size_t bytesSent = send(clientSocket, AUTHENTICATE, sizeof(AUTHENTICATE), 0);

	    if (bytesSent == -1)
		{
		    perror("Send failed");
			close(clientSocket);
			exit(EXIT_FAILURE);
		}
	}
}

