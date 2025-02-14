#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"
#include <cctype>
#include <csignal>
#include <algorithm>
#include <errno.h>

static int serverSocket = -1;
static Server *staticServer = NULL;

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
                }
            }
            for (size_t i = 0; i < sockets.size(); ++i) {
                DeleteClient(sockets[i], *staticServer);
            }
        }
        staticServer->~Server();
        exit(EXIT_SUCCESS);
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

void StartServer(Server& server) {
    InitializeServer(server);
    
    struct pollfd fds[MAX_CLIENTS + 1];
    int nfds = 1;
    
    SetupPoll(fds);

    char buffer[BUFFER_SIZE];

    while (true) {
        if (!PollSockets(fds, nfds)) {
            break;
        }

        HandleNewConnection(fds, nfds, server);
        ProcessClientSockets(fds, nfds, server, buffer);
    }

    close(serverSocket);
}

void InitializeServer(Server& server) {
    staticServer = &server;
    signal(SIGINT, signalHandler);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Socket creation failed");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(server.GetPort());
    server_address.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(serverSocket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
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
}

void SetupPoll(struct pollfd* fds) {
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;

    for (int i = 1; i < MAX_CLIENTS; i++) {
        fds[i].fd = -1;
        fds[i].revents = 0;
    }
}

bool PollSockets(struct pollfd* fds, int nfds) {
    int poll_count = poll(fds, nfds, 100);
    if (poll_count < 0) {
        if (errno == EINTR)
            return false;

        perror("Poll failed");
        return false;
    }
    return true;
}

void HandleNewConnection(struct pollfd* fds, int& nfds, Server& server) {
    if (fds[0].revents & POLLIN && nfds < MAX_CLIENTS) {
        sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&client_address, &client_len);

        if (clientSocket < 0) {
            perror("Accept failed");
            return;
        }

        server.AddClient(clientSocket);
        std::cout << "New connection accepted.\n";

        for (int i = 1; i < MAX_CLIENTS; i++) {
            if (fds[i].fd == -1) {
                fds[i].fd = clientSocket;
                fds[i].events = POLLIN;
                nfds = std::max(nfds, i + 1);
                break;
            }
        }
    }
}

void ProcessClientSockets(struct pollfd* fds, int nfds, Server& server, char* buffer) {
    for (int i = 1; i < nfds; i++) {
        if (fds[i].fd != -1 && fds[i].revents & POLLIN) {
            int clientSocket = fds[i].fd;
            int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            if (bytesReceived > 0) {
                HandleClientMessage(clientSocket, server, buffer, bytesReceived);
            } else {
                HandleClientDisconnection(fds, i, clientSocket, server, server.FindClient(clientSocket)->GetCurrentChannel());  
            }
        }
    }
}

void HandleClientMessage(int clientSocket, Server& server, char* buffer, int bytesReceived) {
    buffer[bytesReceived] = '\0';

    Client* client = server.FindClient(clientSocket);
    if (!client) {
        std::cerr << "Client not found for socket: " << clientSocket << std::endl;
        return;
    }

    client->GetCommandBuffer() += buffer;

    size_t newlinePos;
    while ((newlinePos = client->GetCommandBuffer().find('\n')) != std::string::npos) {
        std::string command = client->GetCommandBuffer().substr(0, newlinePos);
        client->GetCommandBuffer().erase(0, newlinePos + 1);

        if (IsValidMessage(command.c_str())) {
            ParseMessage(command.c_str(), &server, clientSocket);
        } else {
            std::cerr << "Invalid message received: " << command << std::endl;
        }
    }
}

void HandleClientDisconnection(struct pollfd* fds, int index, int clientSocket, Server& server, std::string channelName) {
    std::cout << "Client disconnected.\n";
    Client* client = server.FindClient(clientSocket);
    if (client && !channelName.empty()) {
        Channel* channel = server.GetChannel(channelName);
        if (channel) {
            channel->RemoveMember(client->getNickname());
            std::string msg = ":" + client->getNickname() + " QUIT\n";
            channel->broadcast(msg, NULL);
        }
    }
    DeleteClient(clientSocket, server);
    if (clientSocket != -1) {
        close(clientSocket);
    }
    fds[index].fd = -1;
}

