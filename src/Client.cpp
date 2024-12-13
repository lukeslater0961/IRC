/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsoloher <tsoloher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 14:21:18 by tsoloher          #+#    #+#             */
/*   Updated: 2024/12/13 15:25:27 by tsoloher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"


Client::Client()
{
    _socket = -1;
    _password = false;
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

