/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsoloher <tsoloher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:54:27 by tsoloher          #+#    #+#             */
/*   Updated: 2024/12/15 15:57:47 by tsoloher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"


Channel::Channel(const std::string &name)
{
    _name = name;
    _topic = "";
    _password = "";
    _userLimit = -1;
    _inviteOnly = false;
}

Channel::~Channel()
{
    // _members.clear();
    // _operators.clear();
}


void Channel::AddMember(Client *client)
{
    if (!HasMember(client->getNickname()))
        _members[client->getNickname()] = client;
}

void Channel::RemoveMember(const std::string &nickname)
{
    _members.erase(nickname);
}

bool Channel::HasMember(const std::string &nickname) const
{
    return _members.find(nickname) != _members.end();
}


void Channel::AddOperator(Client *client)
{
    if (!HasOperator(client->getNickname()))
        _operators[client->getNickname()] = client;
}

void Channel::RemoveOperator(const std::string &nickname)
{
    _operators.erase(nickname);
}


bool Channel::HasOperator(const std::string &nickname) const
{
    return _operators.find(nickname) != _operators.end();
}