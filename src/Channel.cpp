/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsoloher <tsoloher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 15:54:27 by tsoloher          #+#    #+#             */
/*   Updated: 2025/01/07 13:11:54 by lslater          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"
#include "../includes/Client.hpp"
#include "../includes/Utils.hpp"
#include <vector>
#include <string>

Channel::Channel(const std::string &name)
{
    _name = name;
    _topic = "";
    _password = "";
    _userLimit = -1;
    _inviteOnly = false;
    _topicRestriction = false;
}

Channel::~Channel()
{
    // _members.clear();
    // _operators.clear();
}


void Channel::AddMember(Client *client)
{
    if (!HasMember(client->getNickname()))
    {
        _members[client->getNickname()] = client;
        if (_members.size() == 1)
            _operators[client->getNickname()] = client;
    }
}

void Channel::RemoveMember(const std::string &nickname)
{
    _members.erase(nickname);
}

bool Channel::HasMember(const std::string &nickname) const
{
    return _members.find(nickname) != _members.end();
}

void Channel::AddOperator(std::string nickname)
{
    if (!HasOperator(nickname))
        _operators[nickname] = _members[nickname];
}

void Channel::RemoveOperator(const std::string &nickname)
{
    _operators.erase(nickname);
}


bool Channel::HasOperator(const std::string &nickname) const
{
    return _operators.find(nickname) != _operators.end();
}


void Channel::broadcast(const std::string &message, Client *client)
{
    for (std::map<std::string, Client *>::iterator it = _members.begin(); it != _members.end(); ++it)
    {
        if (it->second != client )
            SendMsg(it->second, message);
    }
}

std::string Channel::GetMemberList() const
{
    std::string memberList = "";
    for (std::map<std::string, Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
        if (Channel::HasOperator(it->first))
            memberList += "@";
        memberList += it->first + " ";
    }
    return memberList;
}

void	Channel::RemoveInvitedMember(const std::string &nickname)
{
	_invitedMembers.erase(nickname);
}

bool Channel::HasInvitedMember(const std::string &nickname) const
{
    return _invitedMembers.find(nickname) != _invitedMembers.end();
}

void	Channel::AddToInvited(Client *client)
{
    if (!HasInvitedMember(client->getNickname()))
        _invitedMembers[client->getNickname()] = client;
}

bool Channel::isInvited(std::string nickname)
{
	if (!HasInvitedMember(nickname))
		return false;
	else
		return true;
}
