/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 18:33:03 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/06 20:49:57 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../IRC.hpp"

void	IRC::join(Command const &cmd, std::vector<t_clientCmd> &responseQueue)
{
	User				*user(cmd._user);
	string				resp;
	std::vector<string>	chanNames, chanKeys;

	if (cmd._params.empty())
	{
		resp = getResponseFromCode(user, ERR_NEEDMOREPARAMS, (string[]){ cmd._type });
		pushToQueue(user->_fd, resp, responseQueue);
		return ;
	}

	::strSplit(chanNames, cmd._params[0], ",");

	if (cmd._params.size() > 1)
		::strSplit(chanKeys, cmd._params[1], ",");

	for (size_t i(0); i < chanNames.size(); ++i)
	{
		string const	&name(chanNames[i]);
		string const	&key = (i < chanKeys.size())
							 ? chanKeys[i]
							 : "";

		if (!Channel::isNameLegal(name))
		{
			resp = getResponseFromCode(user, ERR_BADCHANMASK, (string[]){ name });
			pushToQueue(user->_fd, resp, responseQueue);
			continue ;
		}
		
		Channel	*chan(getChannelByName(name));
		int	res(0);

		if (!chan)
			chan = newChannel(name, user);
		else
			res = user->tryJoin(chan, key);

		if (res > 0)
		{
			resp = getResponseFromCode(user, res, (string[]){ name });
			pushToQueue(user->_fd, resp, responseQueue);
		}
		else if (!res)
		{
			appendUserNotif(
				user,
				(string[]){ "JOIN", ":" + name, "" },
				chan->_users,
				responseQueue
			);
			if (!chan->_topic.empty())
				topic(Command(user, "TOPIC " + name), responseQueue);
			names(Command(user, "NAMES " + name), responseQueue);
		}
	}
	return ;
}