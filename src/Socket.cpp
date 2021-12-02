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

Socket::Socket(const all_servers &all_servers, int i) : _reuse_addr(1)
{
	memset((char *)&_val, 0, sizeof(_val));
	_val.sin_family = AF_INET;
	_val.sin_addr.s_addr = htonl(stoi(all_servers[i]->ip()));
	_val.sin_port = all_servers[i]->port();
}

Socket::Socket() {}

Socket::~Socket() {}

Socket::Socket(const Socket &cpy) : _reuse_addr(1)
{
	*this = cpy;
}

Socket &Socket::operator=(const Socket &a)
{
	_Socket_fd = a._Socket_fd;
	_val = a._val;
	_connecting = a._connecting;
	return (*this);
}

int		Socket::setup()
{
	if ((_Socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "error" << std::endl;
		return (1);
	}
	if (setsockopt(_Socket_fd, SOL_SOCKET, SO_REUSEADDR, &_reuse_addr, sizeof(_reuse_addr)) < 0)
	{
		std::cout << "error" << std::endl;
		return (1);
	}
	fcntl(_Socket_fd, F_SETFL, O_NONBLOCK);
	if (bind(_Socket_fd, (struct sockaddr*)&_val, sizeof(_val)) < 0)
	{
		std::cout << "error" << std::endl;
		return (1);
	}
	if (listen(_Socket_fd, 500) < 0) //
	{
		std::cout << "error" << std::endl;
		return (1);
	}
	return (0);
}

int		Socket::getFD()
{
	return (_Socket_fd);
}

std::vector<int>	&Socket::getConnecting()
{
	return (_connecting);
}

void	Socket::new_fd()
{
	int	connect;
	if ((connect = accept(_Socket_fd, NULL, NULL)) < 0)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	fcntl(connect, F_SETFL, O_NONBLOCK);
	_connecting.push_back(connect);
}