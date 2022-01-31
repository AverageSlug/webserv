/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaurids <nlaurids@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 12:06:14 by nlaurids          #+#    #+#             */
/*   Updated: 2021/12/02 12:06:14 by nlaurids          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(const Server &server)
{
	_server = server;
	_setaddr();
	_Socket_fd = -1;
}

Socket::Socket() {}

Socket::~Socket()
{
	close(_Socket_fd);
}

Socket::Socket(const Socket &cpy)
{
	*this = cpy;
}

Socket &Socket::operator=(const Socket &a)
{
	_Socket_fd = a._Socket_fd;
	_val = a._val;
	return (*this);
}

void	Socket::_setaddr()
{
	memset((char *)&_val, 0, sizeof(_val));
	_val.sin_family = AF_INET;
	_val.sin_addr.s_addr = inet_addr(_server.ip().c_str());
	_val.sin_port = htons(_server.port());
}

int		Socket::setup()
{
	if ((_Socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "errora" << std::endl;
		return (1);
	}
	printf("%s %i\n", _server.ip().c_str(), _server.port());
	_setaddr();
	int	reuseaddr = 1;
	if (setsockopt(_Socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) < 0)
	{
		std::cout << "errorb" << std::endl;
		return (1);
	}
	if (bind(_Socket_fd, (struct sockaddr*)&_val, sizeof(_val)) < 0)
	{
		std::cout << "errorc" << std::endl;
		return (1);
	}
	if (listen(_Socket_fd, 1000) < 0) //
	{
		std::cout << "errord" << std::endl;
		return (1);
	}
	return (0);
}

long	Socket::getFD()
{
	return (_Socket_fd);
}

struct	sockaddr_in	&Socket::getVal()
{
	return (_val);
}