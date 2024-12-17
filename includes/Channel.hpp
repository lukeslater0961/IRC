/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsoloher <tsoloher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 15:33:05 by tsoloher          #+#    #+#             */
/*   Updated: 2024/12/17 15:06:22 by tsoloher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
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
#include "Client.hpp"
#include <vector>
#include <map>
class Channel
{
    public:
        Channel(const std::string &name);
        ~Channel();
        
        
        void AddMember(Client *client);        
        void RemoveMember(const std::string &nickname);
        bool HasMember(const std::string &nickname) const;

        void AddOperator(Client *client);
        void RemoveOperator(const std::string &nickname);
        bool HasOperator(const std::string &nickname) const;

        void SetTopic(const std::string &topic) {_topic = topic;};
        std::string GetTopic() const {return _topic;};
        
        void SetPassword(const std::string &password) {_password = password;};
        std::string GetPassword() const{return _password;};

        void setUserLimit(int limit) {_userLimit = limit;};
        int getUserLimit() const {return _userLimit;};

        std::string getCurrentChannel() const {return __currentChannel;};
        void setCurrentChannel(const std::string &channel) {__currentChannel = channel;};
        
        void setInviteOnly(bool mode) {_inviteOnly = mode;};
        bool isInviteOnly() const {return _inviteOnly;};
        void setTopicRestriction(bool mode) {_topicRestriction = mode;}; 
    
        bool isTopicRestricted() const { return _topicRestriction;};    

        std::string getName() const {return this->_name;};
        void broadcast(const std::string &message, Client *client);
		std::map<std::string, Client *> GetMembers(void) const {return _members;};
        std::string GetMemberList(void) const;
        
    private :
        std::string _name;
        std::string _topic;
        std::string _password;
        std::string __currentChannel;
        int _userLimit;
        bool _inviteOnly;
        bool _topicRestriction;
        std::map <std::string , Client *> _members;
        std::map<std::string, Client *> _operators;
};

#endif

