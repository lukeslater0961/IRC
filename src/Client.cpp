#include "../includes/Client.hpp"


Client::Client()
{
    _socket = -1;
    _password = false;
	inChannel = false;
}

Client::~Client()
{
    if (_socket != -1)
        close(_socket);
    std::cout << "client destroyed" << std::endl;
}

void Client::StopClient()
{
    if (_socket != -1)
        close(_socket);
    _nickname = "";
    _username = "";
    _socket = -1;
    _password = false;
}

