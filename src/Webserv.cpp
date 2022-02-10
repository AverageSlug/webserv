#include "Webserv.hpp"

Webserv::Webserv(const all_servers &all_servers)
{
	_all_servers = all_servers;
}

Webserv::Webserv() {}

Webserv::~Webserv()
{
	delete [] _Socket;
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
			std::cout << "\r" << std::flush;
		else
			_handle_fd_set();
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

const Server*	Webserv::getReqServ(const std::string name) const
{
	for (size_t i = 0; i < _all_servers.size(); ++i)
	{
		for (size_t j = 0; j < _all_servers[i]->name().size(); ++j)
		{
			if (_all_servers[i]->name()[j] == name)
				return _all_servers[i];
		}
	}
	return _all_servers[0];
}

int		Webserv::reqParser()
{
	std::vector<std::string>				buffer = ft_strtovec(_Request->getContent(), "\n");
	std::vector<std::string>::iterator		line = buffer.begin();

	if (_Request->setRequestUri(*line++) == false)
	{
		_Request->setServer(getReqServ(""));
		return 0;
	}
	for ( ; line != buffer.end() && !(*line).empty(); ++line)
		_Request->setHeaderData(*line);
	_Request->setServer(getReqServ(_Request->getData()["Host"][0]));
	_Request->setConstructPath();
	_Request->setChunked();
//	setContent();
	return 1;
}

void	Webserv::_handle_fd_set()
{
	int v = 1;
	std::string to_send;
	
	for (std::vector<long>::iterator it = _connected.begin(); it != _connected.end(); it++)
	{
		if (FD_ISSET(*it, &_write_fd))
		{
			_Response = new Response(_Request);
			_Response->setContent(getFileContent(_Request->getConstructPath()));
			_Response->header();
			to_send = _Response->get_header();
			if (!_Request->getLocation()->cgi.first.length() || ft_checkDir(_Request->getConstructPath()))
				to_send += "\r\n";
			to_send += _Response->getContent();
			std::cout << "Method " << _Request->getMethod() << " : " << _Request->getConstructPath() << " sent with status code " << _Response->getStatus().first << " " << _Response->getStatus().second << std::endl << std::endl;
			if (send(*it, to_send.c_str(), to_send.length(), 0) < 0)
			{
				FD_CLR(*it, &_set);
				FD_CLR(*it, &_read_fd);
				_connecting.erase(*it);
			}
			_connected.erase(it);
			delete _Response;
			delete _Request;
			v = 0;
			break ;
		}
	}
	for (std::map<long, long>::iterator it = _connecting.begin(); v && it != _connecting.end(); it++)
	{
		int ret = 0;
		long sock = it->first;
		if (FD_ISSET(sock, &_read_fd))
		{
			char buff[6001] = {0};
			ret = recv(sock, buff, 6000, 0);
			if (ret <= 6)
			{
				if (sock > 0)
					close(sock);
				FD_CLR(sock, &_set);
				FD_CLR(sock, &_read_fd);
				_connecting.erase(sock);
				it = _connecting.begin();
				break ;
			}
			_Request = new Request(std::string(buff));
//			size_t reqLen = requestLen(_Request->getContent());
			if (ret >= 6000)
			{
				_Request->setStatus(413);
			}
			if (reqParser() == 0 || (std::string(buff).compare(0, 3, "GET") && std::string(buff).compare(0, 4, "POST") && std::string(buff).compare(0, 6, "DELETE") && std::string(buff).compare(0, 4, "----")))
				_Request->setStatus(400);
			_connected.push_back(sock);
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
			_connecting.insert(std::make_pair(new_socket, _Socket[i].getFD()));
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
