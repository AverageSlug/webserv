/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaurids <nlaurids@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 12:06:17 by nlaurids          #+#    #+#             */
/*   Updated: 2021/12/02 12:06:17 by nlaurids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "includes.hpp"
# include "all_servers.hpp"

class Socket
{
	public:
		void	set_Socket(const Server &server);
		Socket();
		~Socket();
		Socket(const Socket &cpy);
		Socket &operator=(const Socket &a);
		int		setup();
		long	getFD();
		struct	sockaddr_in	&getVal();
	private:
		void	_setaddr();
		long	_Socket_fd;
		Server	_server;
		struct	sockaddr_in	_val;
};

#endif