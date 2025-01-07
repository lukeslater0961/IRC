/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsoloher <tsoloher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:21:18 by tsoloher          #+#    #+#             */
/*   Updated: 2025/01/07 11:10:06 by tsoloher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// bool Client::isInvited(std::string channelName)
// {
//     for (std::vector<std::string>::iterator it = _invitedChannels.begin(); it != _invitedChannels.end(); it++)
//     {
//         if (*it == channelName)
//             return true;
//     }
//     return false;
// }
