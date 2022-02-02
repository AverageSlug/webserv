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

Webserv::~Webserv()
{
	delete [] _Socket;
	delete _Request;
}

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
	FD_ZERO(&_set);
	_size = _all_servers.size();
	_server_fd_highest = 0;
	_Socket = new Socket[_size];
	for (int i = 0; i < _size; i++)
	{
		_Socket[i].set_Socket(*_all_servers.getservs()[i]);
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
	for (std::vector<long>::iterator it = _connected.begin(); it < _connected.end(); it++)
		if (FD_ISSET(*it, &_write_fd))
		{
			_Response = new Response(_Request);
			if (send(*it, _Response->header().c_str(), _Response->header().length(), 0) < 0)
				throw "Error: send";
			FD_CLR(*it, &_write_fd);
			FD_SET(*it, &_set);
			_connected.erase(it);
			it = _connected.begin();
			_Response->setContent(getContent(_Request->getConstructPath()));
		}
	for (std::vector<long>::iterator it = _connecting.begin(); it < _connecting.end(); it++)
	{
		if (FD_ISSET(*it, &_read_fd))
		{
			char buff[65536];
			int	ret;
			ret = recv(*it, buff, 65535, 0);
			if (ret < 0)
				throw "Error: recv";
			if (ret > 0)
			{
				std::cout << std::string(buff) << std::endl;
				_Request = new Request(std::string(buff), _all_servers);
				_Request->reqParser();
				_connected.push_back(*it);
				FD_CLR(*it, &_set);
				FD_CLR(*it, &_read_fd);
				_connecting.erase(it);
				it = _connecting.begin();
			}
		}
	}
	for (unsigned int i = 0; i < _all_servers.size(); i++)
	{
		if (FD_ISSET(_Socket[i].getFD(), &_read_fd))
		{
			long	new_socket = accept(_Socket[i].getFD(), NULL, NULL);
			if (new_socket < 0)
				throw "Error: accept";
			fcntl(new_socket, F_SETFL, O_NONBLOCK);
			FD_SET(new_socket, &_set);
			_connecting.push_back(new_socket);
			if (new_socket > _server_fd_highest)
				_server_fd_highest = new_socket;
		}
	}
}

all_servers	&Webserv::getAllServers()
{
	return (_all_servers);
}

Socket	*Webserv::getSocket()
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