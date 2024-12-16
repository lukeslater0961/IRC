/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsoloher <tsoloher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:21:21 by tsoloher          #+#    #+#             */
/*   Updated: 2024/12/16 13:03:18 by lslater          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

		bool	inChannel;

	private:
		std::string _currentChannel;
		std::string _nickname;
		std::string _username;
		bool          _password;
		int           _socket;
};


#endif
