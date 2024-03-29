/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   version.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mahautlatinis <mahautlatinis@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 19:16:03 by mahautlatin       #+#    #+#             */
/*   Updated: 2023/10/13 19:35:53 by mahautlatin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <IRC.hpp>

void	IRC::version(Command const &cmd,
	std::vector<t_clientCmd> &responseQueue)
{
	User		*user(cmd._user);
	std::string	resp(getResponseFromCode(user, RPL_VERSION, NULL));

	pushToQueue(user->_fd, resp, responseQueue);
	return ;
}
