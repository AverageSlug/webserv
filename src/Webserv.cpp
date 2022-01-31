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

Webserv::Webserv(const all_servers &all_servers)
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
			throw "Socket setup failed";
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
		FD_ZERO(&_write_fd);
		for (std::vector<long>::iterator it = _connected.begin(); it < _connected.end(); it++)
			FD_SET(*it, &_write_fd);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if ((select_fd = select(_server_fd_highest + 1, &_read_fd, &_write_fd, NULL, &timeout)) < 0)
			throw "Error: select";
		if (!select_fd)
			std::cout << "\r..." << std::flush;
		else
			_handle_fd_set();
	}
}

void	Webserv::_handle_fd_set()
{
	for (std::vector<long>::iterator it = _connecting.begin(); it < _connecting.end(); it++)
	{
		if (FD_ISSET(*it, &_read_fd))
		{
			char buff[65536];
			if (recv(*it, buff, 65535, 0) <= 0)
				throw "Error: recv";
			printf("HERE1\n");
			Request request("lo", _all_servers);
			printf("HERE2\n");
			_connected.push_back(*it);
			printf("HERE3\n");
			FD_CLR(*it, &_set);
			printf("HERE4\n");
			FD_CLR(*it, &_read_fd);
			printf("HERE5\n");
			_connecting.erase(it);
			printf("HERE6\n");
			it = _connecting.begin();
			printf("HERE7\n");
		}
	}
	printf("HEREn\n");
	for (std::vector<long>::iterator it = _connected.begin(); it < _connected.end(); it++)
		if (FD_ISSET(*it, &_read_fd))
		{
			std::cout << "jsptest" << std::endl;
		}
	printf("HEREm\n");
	for (unsigned int i = 0; i < _all_servers.size(); i++)
	{
		if (FD_ISSET(_Socket[i].getFD(), &_read_fd))
		{
			std::cout << "\nnew connection" << std::endl;
			long	new_socket = accept(_Socket[i].getFD(), NULL, NULL);
			if (new_socket < 0)
				throw "Error: accept";
			fcntl(new_socket, F_SETFL, O_NONBLOCK);
			FD_SET(new_socket, &_set);
			_connecting.push_back(new_socket);
			if (new_socket > _server_fd_highest)
				_server_fd_highest = new_socket;
		}
		printf("HERE0\n");
	}
}

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