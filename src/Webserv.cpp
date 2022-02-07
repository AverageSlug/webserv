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
		memcpy(&_read_fd, &_set, sizeof(_set));
		FD_ZERO(&_write_fd);
		for (std::vector<long>::iterator it = _connected.begin(); it < _connected.end(); it++)
			FD_SET(*it, &_write_fd);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if ((select_fd = select(_server_fd_highest + 1, &_read_fd, &_write_fd, NULL, &timeout)) < 0)
			throw "Error: select";
		if (!select_fd)
			std::cout << "..." << std::flush;
		else
		{
			std::cout << "\r";
			_handle_fd_set();
		}
	}
}

size_t	Webserv::requestLen(std::string const & content)
{
	size_t pos = content.find("Content-Length: ");

	if (pos == std::string::npos)
		return std::string::npos;
	
	pos += std::string("Content-Length: ").length();

	std::string getLen;

	for (size_t i = pos; i < content.length() - pos; i++)
	{
		if (std::isdigit(content[i]))
			getLen += content[i];
		else
			break ;
	}

	std::stringstream sstream(getLen);
	size_t ret = 0;

	sstream >> ret;
	return ret;
}

void	Webserv::_handle_fd_set()
{
	int v = 1;
	std::string to_send;
	
	for (std::vector<long>::iterator it = _connected.begin(); it < _connected.end(); it++)
	{
		if (FD_ISSET(*it, &_write_fd))
		{
			_Response = new Response(_Request);
			_Response->setContent(getFileContent(_Request->getConstructPath()));
			_Response->header();
			to_send = _Response->get_header();
			to_send += _Response->getContent();
			if (send(*it, to_send.c_str(), to_send.length(), 0) < 0)
				throw "Error: send";
			std::cout << "Response sent!" << std::endl;
			_connected.erase(it);
			v = 0;
			break ;
		}
	}
	for (std::vector<long>::iterator it = _connecting.begin(); v && it < _connecting.end(); it++)
	{
		int ret = 0;
		if (FD_ISSET(*it, &_read_fd))
		{
			char *buff = new char[2500];
			std::memset(buff, 0, 2500);
			ret = recv(*it, buff, 2500, 0);
			if (ret == 0)
			{
				FD_CLR(*it, &_set);
				FD_CLR(*it, &_read_fd);
				_connecting.erase(it);
				break ;
			}
			else if (ret < 0)
				std::cout << "ERRNO ALLERTE ALLERTE ALLERTE BUGGGGGGGGGGGGGGGGGGGGGGGGG: " << errno << std::endl;
			std::cout << buff << std::endl;
			_Request = new Request(std::string(buff), _all_servers);
			size_t reqLen = requestLen(_Request->getContent());
			if (reqLen > 2500 && reqLen != std::string::npos)
				_Request->setStatus(413);
			if (_Request->reqParser() == 0)
				_Request->setStatus(400);
			_connected.push_back(*it);
			delete [] buff;
			break ;
		}
	}
	for (unsigned int i = 0; v && i < _all_servers.size(); i++)
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
			break ;
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