#include "Socket.hpp"

void	Socket::set_Socket(const Server &server)
{
	_server = server;
	_setaddr();
	_Socket_fd = -1;
}

Socket::Socket() {}

Socket::~Socket()
{
}

Socket::Socket(const Socket &cpy)
{
	*this = cpy;
}

Socket &Socket::operator=(const Socket &a)
{
	_server = a._server;
	_setaddr();
	_Socket_fd = a._Socket_fd;
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
		std::cout << "Error: socket" << std::endl;
		return (1);
	}
	printf("%s %i\n", _server.ip().c_str(), _server.port());
	_setaddr();
	int	reuseaddr = 1;
	if (setsockopt(_Socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) < 0)
	{
		std::cout << "Error: setsockopt" << std::endl;
		return (1);
	}
	fcntl(_Socket_fd, F_SETFL, O_NONBLOCK);
	if (bind(_Socket_fd, (struct sockaddr*)&_val, sizeof(_val)) < 0)
	{
		std::cout << "Error: bind" << std::endl;
		return (1);
	}
	if (listen(_Socket_fd, 1000) < 0)
	{
		std::cout << "Error: listen" << std::endl;
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
