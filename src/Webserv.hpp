/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaurids <nlaurids@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 12:06:30 by nlaurids          #+#    #+#             */
/*   Updated: 2021/12/02 12:06:30 by nlaurids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "includes.hpp"
# include "Socket.hpp"

class Webserv
{
	public:
		Webserv(all_servers all_servers);
		Webserv();
		~Webserv();
		Webserv(const Webserv &cpy);
		Webserv &operator=(const Webserv &a);
		void	setup();
		void	server();
		all_servers	&getAllServers();
		Socket		*getSocket();
		long		getHighestFD();
		fd_set		getReadFD();
		fd_set		getWriteFD();
	private:
		all_servers	_all_servers;
		Socket	*_Socket;
		long	_server_fd_highest;
		fd_set	_set;
		int		_size;
		fd_set	_read_fd;
		fd_set	_write_fd;
//		void	_handle_fd_set();
};

#endif