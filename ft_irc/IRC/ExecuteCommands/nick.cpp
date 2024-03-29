/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:54:48 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/13 19:34:42 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRC.hpp>

void	IRC::nick(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User		*user(cmd._user);
	std::string	resp;
	bool		newUser(false);

	if (cmd._params.empty())
	{
		resp = getResponseFromCode(user, ERR_NONICKNAMEGIVEN, NULL);
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	std::string const	&nick(cmd._params[0]);

	if (!User::checkNickValidChars(nick))
		resp = getResponseFromCode(user,
			ERR_ERRONEUSNICKNAME, (std::string[]){ nick });
	else if (nick == user->_nick)
		return;
	else if (getUserByNick(nick))
		resp = getResponseFromCode(user, 
			ERR_NICKNAMEINUSE, (std::string[]){ nick });
	else
	{
		newUser = !(user->_registered);
		if (!newUser)
			resp = appendUserNotif(
				user,
				(std::string[]){ "NICK", ":" + nick, "" },
				getCommonUsers(user),
				responseQueue
			);
		user->setNick(nick);
	}
	if (!newUser)
		pushToQueue(user->_fd, resp, responseQueue);
	return ;
}
