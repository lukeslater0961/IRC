#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include <cctype>
#include <csignal>
#include <algorithm>
#include <errno.h>

static int serverSocket = -1;
static Server *staticServer = NULL;
bool keepRunning = true;
Server::Server() {
}
Server::~Server() {
   for (std::vector<Client*>::iterator it = this->client.begin(); it != this->client.end(); ++it) {
        delete *it;
    }
    this->client.clear(); // Ensure the vector is empty
    for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
        delete it->second;
    }
    _channels.clear();
    
}

void Server::AddClient(int clientSocket)
{
	Client *newClient = new Client();
    if (!newClient)
    {
        perror("Memory allocation failed");
        if (clientSocket != -1)
            close(clientSocket);
        return;
    }
    newClient->SetSocket(clientSocket);
    this->client.push_back(newClient);
}

bool IsValidMessage(const char* message)
{
    if (message == NULL || strlen(message) == 0 || strlen(message) > BUFFER_SIZE - 1)
        return false;
    return true;
}

void	DeleteClient(int clientSocket, Server &server)
{
	Client *client = server.FindClient(clientSocket);
    if (client)
    {
        std::vector<Client *>::iterator it = server.client.begin();
        while (it != server.client.end())
        {
            if (*it == client)
            {
                it = server.client.erase(it); // Remove from the list
                break;
            }
            else
                ++it;
        }
        delete client;
    }
    else
        std::cerr << "Client not found for socket: " << clientSocket << std::endl;
}

void signalHandler(int signal)
{
    if (signal == SIGINT) {
        std::cout << "\nCtrl+C detected. Shutting down server gracefully..." << std::endl;
        if (serverSocket != -1) {
            close(serverSocket);
            std::cout << "Server socket closed." << std::endl;
        }
    
        if (staticServer) {
            std::vector<int> sockets;
            for (std::vector<Client*>::iterator it = staticServer->client.begin(); it != staticServer->client.end(); ++it) {
                if (*it) {
                    std::cout << "Cleaning up client with socket: " << (*it)->GetSocket() << std::endl;
                    sockets.push_back((*it)->GetSocket());
                    if ((*it)->GetSocket() != -1)
                        close((*it)->GetSocket());
                    (*it)->StopClient();
                }
            }
            for (size_t i = 0; i < sockets.size(); ++i) {
                DeleteClient(sockets[i], *staticServer);
            }
        }
        keepRunning = false;
    }
}


Client* Server::FindClient(int clientSocket)
{
    std::vector<Client *>::iterator it;
    for (it = this->client.begin(); it != this->client.end(); ++it)
    {
        if (*it && (*it)->GetSocket() == clientSocket)
            return (*it);
    }
    return NULL;
}

int SetupServer(char **argv)
{
    std::string port = argv[1];
    std::string password = argv[2];
    Server server;
    int portNum = 0;
    for (size_t i = 0; i < port.length(); i++) {
        if (!std::isdigit(port[i]))
            return (ErrorMngment(INVALID_ARGS));
    }
    portNum = std::atoi(port.c_str());

    if (port.empty() || password.empty() || portNum == 0 || portNum > 65535)
        return (ErrorMngment(INVALID_ARGS));

    server.SetPort(portNum);
    server.SetPassword(argv[2]);
    std::cout << "Password: " << server.GetPassword() << std::endl
              << "Port: " << server.GetPort() << std::endl; // to be removed
    StartServer(server);
    return (0);
}

void Server::CreateChannel(const std::string &name)
{
   if (_channels.find(name) == _channels.end()) {
        _channels[name] = new Channel(name);
        std::cout << "Channel " << name << " created." << std::endl;
    }
}

Channel* Server::GetChannel(const std::string &name)
{
  std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (it != _channels.end())
        return it->second;
    return NULL;
}

void Server::DeleteChannel(const std::string &name)
{
    std::map<std::string, Channel*>::iterator it = _channels.find(name);
    if (it != _channels.end()) {
        delete it->second;
        _channels.erase(it);
        std::cout << "Channel " << name << " deleted." << std::endl;
    } else
        std::cerr << "Error: Channel " << name << " not found." << std::endl;
}

void StartServer(Server& server)
{
    staticServer = &server;
    signal(SIGINT, signalHandler);
	int opt = 1;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1)
    {
        perror("Socket creation failed");
        close(serverSocket);
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
       {
        fds[i].revents = 0;          // Initialize client slots as unused
        fds[i].fd = -1;          // Initialize client slots as unused
       }
    
    char buffer[BUFFER_SIZE];

    while (keepRunning)
    {
        int poll_count = poll(fds, nfds, 100); // Wait for an event
        if (poll_count < 0)
        {
            if (errno == EINTR)
                break;
            else {
                perror("Poll failed");
                break;
            }
        }

        if (fds[0].revents & POLLIN)
        {
            sockaddr_in client_address;
            socklen_t client_len = sizeof(client_address);
            int clientSocket = accept(serverSocket, (struct sockaddr *)&client_address, &client_len);
            server.AddClient(clientSocket);
            if (clientSocket < 0)
            {
                perror("Accept failed");
                continue;
            }

            std::cout << "New connection accepted.\n";

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
                {buffer[bytesReceived] = '\0';

            Client *client = server.FindClient(clientSocket);
            if (!client) {
                std::cerr << "Client not found for socket: " << clientSocket << std::endl;
                continue;
            }

            client->GetCommandBuffer() += buffer;
            
            size_t newlinePos;
            while ((newlinePos = client->GetCommandBuffer().find('\n')) != std::string::npos) {
                std::string command = client->GetCommandBuffer().substr(0, newlinePos);
				if (!command.empty())
					client->GetCommandBuffer().erase(0, newlinePos + 1);

                if (IsValidMessage(command.c_str())) {
                    ParseMessage(command.c_str(), &server, clientSocket);
                } else
                    std::cerr << "Invalid message received: " << command << std::endl;
            }
                }
                else if (bytesReceived == 0)
                {
                    std::cout << "Client disconnected.\n";
					DeleteClient(clientSocket, server);
                    if (clientSocket != -1)
                        close(clientSocket);
                    fds[i].fd = -1;
                }
                else
                {
                    perror("Recv failed");
                    if (clientSocket != -1)
                        close(clientSocket);
                    fds[i].fd = -1;
                }
            }
        }
    }
    close(serverSocket);
}
