/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsoloher <tsoloher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 15:33:05 by tsoloher          #+#    #+#             */
/*   Updated: 2024/12/16 13:10:38 by lslater          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Utils.hpp"

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

        void setInviteOnly(bool mode) {_inviteOnly = mode;};
        bool isInviteOnly() const {return _inviteOnly;};
        void setTopicRestriction(bool mode) {_topicRestriction = mode;}; 
    
        bool isTopicRestricted() const { return _topicRestriction;};    

        std::string getName() const {return _name;};
		std::map<std::string, Client *> GetMembers(void){return (this->_members);}
    
    private :
        std::string _name;
        std::string _topic;
        std::string _password;
        int _userLimit;
        bool _inviteOnly;
        bool _topicRestriction;
        std::map <std::string , Client *> _members;
        std::map<std::string, Client *> _operators;
};

#endif

