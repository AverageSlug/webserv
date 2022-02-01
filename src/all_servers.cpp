/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_servers.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaurids <nlaurids@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 00:21:38 by igor              #+#    #+#             */
/*   Updated: 2022/01/31 17:49:06 by nlaurids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_servers.hpp"

all_servers::all_servers()
{
	_bracket[OUTSIDE] = WAIT_BRACKS;
	_bracket[IN_SERVER] = NONE;
	_bracket[IN_LOCA] = NONE;
}

all_servers::~all_servers()
{
	for (size_t i = 0; i < this->_servers.size(); i++)
		delete this->_servers[i];
	this->_servers.clear();
}

all_servers::all_servers(const all_servers &x)
{
	*this = x;
}

all_servers&	all_servers::operator=(const all_servers &x)
{
	if (&x == this)
		return *this;
	this->_servers = x._servers;
	this->_bracket = x._bracket;
	return *this;
}

size_t	all_servers::size()  const
{
	return this->_servers.size();
}

Server*	all_servers::operator[](int i) const
{
	return this->_servers[i];
}

Server*	all_servers::last()            const
{
	return this->_servers.back();
}

void	all_servers::newServer(const data_type &data)
{
	(void)data; // !
	this->_servers.push_back(new Server());
}

void	all_servers::newLocation(Server *server, const data_type &data)
{
	server->newLocation(data);
	_bracket[IN_LOCA] = WAIT_BRACKS;
}

void	all_servers::newParserLine(Server *server, const data_type &data)
{
	if (_bracket[IN_LOCA] == IN_BRACKS)
		server->newLocationDirective(data);
	else if (_bracket[IN_SERVER] == IN_BRACKS)
		server->newDirective(data);
	else
		throw "Error while reading configuration file";
}

void	all_servers::openBlock(const data_type &data)
{
	if (data.size() != 1)
		throw "Error while reading configuration file";
	if (_bracket[IN_SERVER] == WAIT_BRACKS)
	{
		_bracket[IN_SERVER] = IN_BRACKS;
		_bracket[OUTSIDE] = NONE;
	}
	else if (_bracket[IN_LOCA] == WAIT_BRACKS)
		_bracket[IN_LOCA] = IN_BRACKS;
	else
		throw "Error while reading configuration file";
}

void	all_servers::closeBlock(const data_type &data)
{
	if (data.size() != 1)
		throw "Error while reading configuration file";

	if (_bracket[IN_LOCA] == IN_BRACKS)
		_bracket[IN_LOCA] = NONE;
	else if (_bracket[IN_SERVER] == IN_BRACKS)
	{
		_bracket[IN_SERVER] = NONE;
		_bracket[OUTSIDE] = WAIT_BRACKS;
	}
	else
		throw "Error while reading configuration file";
}

std::ostream&	operator<<(std::ostream& os, const all_servers& servers)
{
	for (size_t i = 0; i < servers.size(); i++)
		os << *(servers[i]);
	return os;
}