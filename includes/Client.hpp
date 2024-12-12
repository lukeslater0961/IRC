/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsoloher <tsoloher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:21:21 by tsoloher          #+#    #+#             */
/*   Updated: 2024/12/12 12:04:39 by lslater          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Utils.hpp"

class Client{
    public:

        Client();
        ~Client();
        void StopClient();
        std::string getNickname(){return (this->_nickname);}
        std::string getUsername(){return (this->_username);}
        void setNickname(std::string nickname){this->_nickname = nickname;};
        void setUsername(std::string username){this->_username = username;};

    private:
      std::string _nickname;
      std::string _username;
      bool          _password;
      int           _socket;
};


#endif
