/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaurids <nlaurids@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 12:06:24 by nlaurids          #+#    #+#             */
/*   Updated: 2021/12/02 12:06:24 by nlaurids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

Webserv::Webserv(all_servers all_servers)
{
	_all_servers = all_servers;
}

Webserv::Webserv() {}

Webserv::~Webserv() {}

Webserv::Webserv(const Webserv &cpy)
{
	*this = cpy;
}

Webserv &Webserv::operator=(const Webserv &a)
{
	(void)a;
	return (*this);
}

void	Webserv::setup()
{
	_Socket = new Socket[_all_servers.size()];
	for (unsigned int i = 0; i < _all_servers.size(); i++)
	{
		Socket	Socket(_all_servers, i);
		if (Socket.setup())
		{
			std::cout << "error" << std::endl;
			exit(1);
		}
		_Socket[i] = Socket;
		if (_Socket[i].getFD() > _server_fd_highest)
			_server_fd_highest = _Socket[i].getFD();
	}
}

void	Webserv::server()
{
	struct	timeval	timeout;
	int		select_fd;

	while (1)
	{
		_fd_set_set();
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if ((select_fd = select(_server_fd_highest + 1, &_read_fd, &_write_fd, NULL, &timeout)) < 0)
		{
			std::cout << "error" << std::endl;
			exit(1);
		}
		if (!select_fd)
			std::cout << "waiting" << std::endl;
		else
			_handle_fd_set();
	}
}

void	Webserv::_fd_set_set()
{
	FD_ZERO(&_read_fd);
	FD_ZERO(&_write_fd);
	for (unsigned int i = 0; i < _all_servers.size(); i++)
	{
		FD_SET(_Socket[i].getFD(), &_read_fd);
		FD_SET(_Socket[i].getFD(), &_write_fd);
		for (std::vector<int>::iterator it = _Socket[i].getConnecting().begin(); it < _Socket[i].getConnecting().end(); it++)
			if (*it)
			{
				FD_SET(*it, &_read_fd);
				FD_SET(*it, &_write_fd);
				if (*it > _server_fd_highest)
					_server_fd_highest = *it;
			}
	}
}

void	Webserv::_handle_fd_set()
{
	for (unsigned int i = 0; i < _all_servers.size(); i++)
	{
		if (FD_ISSET(_Socket[i].getFD(), &_read_fd) || FD_ISSET(_Socket[i].getFD(), &_write_fd))
			_Socket[i].new_fd();
		for (std::vector<int>::iterator it = _Socket[i].getConnecting().begin(); it < _Socket[i].getConnecting().end(); it++)
			if (FD_ISSET(*it, &_read_fd) || FD_ISSET(*it, &_write_fd)) {}
	}
}