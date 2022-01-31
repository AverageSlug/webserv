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
	std::vector<Server *>	servs = _all_servers.getservs();
	FD_ZERO(&_set);
	_size = _all_servers.size();
	_server_fd_highest = 0;
	for (int i = 0; i < _size; i++)
	{
		Socket	Socket(*servs[i]);
		_Socket.push_back(Socket);
		if (_Socket[i].setup())
		{
			std::cout << "erroroo" << std::endl;
			exit(1);
		}
		FD_SET(_Socket[i].getFD(), &_set);
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
		memcpy(&_read_fd, &_set, _size);
//		FD_ZERO(&_write_fd);
//		for (std::vector<long>::iterator it = _Socket[i].getConnecting().begin(); it < _Socket[i].getConnecting().end(); it++)
//			FD_SET(*it, &_write_fd);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if ((select_fd = select(_server_fd_highest + 1, &_read_fd, NULL, NULL, &timeout)) < 0)
		{
			std::cout << "error" << std::endl;
			exit(1);
		}
		if (!select_fd)
			std::cout << "." << std::flush;
		else
			printf("\rconnection acquired");//_handle_fd_set();
	}
}
/*
void	Webserv::_handle_fd_set()
{
	for (unsigned int i = 0; i < _all_servers.size(); i++)
	{
		if (FD_ISSET(_Socket[i].getFD(), &_write_fd)) {}
		printf("connection acquired\n");
		for (std::vector<long>::iterator it = _Socket[i].getConnecting().begin(); it < _Socket[i].getConnecting().end(); it++)
			if (FD_ISSET(*it, &_read_fd)) {}
	}
}
*/
all_servers	&Webserv::getAllServers()
{
	return (_all_servers);
}

std::vector<Socket>	Webserv::getSocket()
{
	return (_Socket);
}

long		Webserv::getHighestFD()
{
	return (_server_fd_highest);
}

fd_set		Webserv::getReadFD()
{
	return (_read_fd);
}

fd_set		Webserv::getWriteFD()
{
	return (_write_fd);
}