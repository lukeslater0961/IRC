#ifndef CLIENT_HPP
#define CLIENT_HPP


#include <string>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netinet/in.h>
#include <poll.h>
#include <cstring>
#include <vector>

class Client{

	public:

		Client();
		~Client();

		void StopClient();

		std::string getNickname(){return (this->_nickname);}
		std::string getUsername(){return (this->_username);}

		void setNickname(std::string nickname){this->_nickname = nickname;};
		void setUsername(std::string username){this->_username = username;};

		void	SetSocket(int socket){this->_socket = socket;}
		int 	GetSocket(){return this->_socket;}

		bool	GetPassword(){return this->_password;};
		void   SetPassword(bool password){this->_password = password;};

		void		SetCurrentChannel(std::string name){this->_currentChannel = name;}
		std::string GetCurrentChannel(void){return (this->_currentChannel);}
		std::string &GetCommandBuffer() { return _commandBuffer; }
    	void ClearCommandBuffer() { _commandBuffer.clear(); }

		bool	inChannel;
	private:
		std::string _currentChannel;
		std::string _nickname;
		std::string _username;
		std::string _commandBuffer;
		bool          _password;
		int           _socket;
};


#endif
