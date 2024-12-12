#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include <cctype>

Server::Server() {}

void Server::AddClient(int clientSocket)
{
    Client *newClient = new Client();
    std::cout << "in add client" << std::endl;
    this->client.push_back(newClient);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (this->client[i]->GetSocket() == -1 && this->client[i]->GetSocket() != clientSocket)
        {
            this->client[i]->SetSocket(clientSocket);
            break;
        }
    }
}

void	DeleteClient(int clientSocket, Server server)
{
	std::cout << "deleting client" << std::endl;
	Client *client = server.FindClient(clientSocket);
	delete(client);
}

Client* Server::FindClient(int clientSocket)
{
    std::vector<Client *>::iterator it;
    for (it = this->client.begin(); it != this->client.end(); it++)
    {
        if ((*it)->GetSocket() == clientSocket)
        {
            std::cout << "gfdgfg`" << std::endl;
            return (*it);
        }
    }
    return NULL;
}


int SetupServer(char **argv)
{
    std::string port = argv[1];
    std::string password = argv[2];
    Server server;

    if (port.empty() || password.empty() || std::atoi(argv[1]) == 0)
        return (ErrorMngment(INVALID_ARGS));

    server.SetPort(std::atoi(argv[1]));
    server.SetPassword(argv[2]);
    std::cout << "Password: " << server.GetPassword() << std::endl
              << "Port: " << server.GetPort() << std::endl; // to be removed
    StartServer(server);
    return (0);
}

void StartServer(Server server)
{
	int opt = 1;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server.GetPort());
    server_address.sin_addr.s_addr = INADDR_ANY;

	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(serverSocket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Bind failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, SOMAXCONN) < 0)
    {
        perror("Listen failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << server.GetPort() << std::endl;

    struct pollfd fds[MAX_CLIENTS];
    int nfds = 1;

    fds[0].fd = serverSocket;    // Server socket
    fds[0].events = POLLIN;      // Monitor for incoming connections

    for (int i = 1; i < MAX_CLIENTS; i++)
        fds[i].fd = -1;          // Initialize client slots as unused

    char buffer[BUFFER_SIZE];

    while (true)
    {
        int poll_count = poll(fds, nfds, -1); // Wait for an event

        if (poll_count < 0)
        {
            perror("Poll failed");
            break;
        }

        // Check for new incoming connections
        if (fds[0].revents & POLLIN)
        {
            sockaddr_in client_address;
            socklen_t client_len = sizeof(client_address);
            int clientSocket = accept(serverSocket, (struct sockaddr *)&client_address, &client_len);
            server.AddClient(clientSocket);
            Client newClient;
            if (clientSocket < 0)
            {
                perror("Accept failed");
                continue;
            }

            std::cout << "New connection accepted.\n";

            // Add client socket to the poll array
            for (int i = 1; i < MAX_CLIENTS; i++)
            {
                if (fds[i].fd == -1)
                {
                    fds[i].fd = clientSocket;
                    fds[i].events = POLLIN; // Monitor for incoming data
                    nfds = std::max(nfds, i + 1);
                    break;
                }
            }
        }

        // Check for events on existing client sockets
        for (int i = 1; i < nfds; i++)
        {
            if (fds[i].fd != -1 && fds[i].revents & POLLIN)
            {
                int clientSocket = fds[i].fd;
                int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

                if (bytesReceived > 0)
                {
                    buffer[bytesReceived] = '\0';
					ParseMessage(buffer, server, clientSocket);
                }
                else if (bytesReceived == 0)
                {
                    std::cout << "Client disconnected.\n";
					DeleteClient(clientSocket, server);
                    close(clientSocket);
                    fds[i].fd = -1;
                }
                else
                {
                    perror("Recv failed");
                    close(clientSocket);
                    fds[i].fd = -1;
                }
            }
        }
    }
    close(serverSocket);
}
