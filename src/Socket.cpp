#include "Socket.hpp"

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
	memset((char *)&_val, 0, sizeof(_val));
	_val.sin_family = AF_INET;
	_val.sin_addr.s_addr = a._val.sin_addr.s_addr;
	_val.sin_port = a._val.sin_port;
	_Socket_fd = a._Socket_fd;
	return (*this);
}

void	Socket::_setaddr(const Server &server)
{
	memset((char *)&_val, 0, sizeof(_val));
	_val.sin_family = AF_INET;
	_val.sin_addr.s_addr = inet_addr(server.ip().c_str());
	_val.sin_port = htons(server.port());
}

int		Socket::setup(const Server &server)
{
	if ((_Socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw "Error: socket";
	printf("%s | %i\n", server.ip().c_str(), server.port());
	_setaddr(server);
	int	reuseaddr = 1;
	if (setsockopt(_Socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) < 0)
		throw "Error: setsockopt";
	fcntl(_Socket_fd, F_SETFL, O_NONBLOCK);
	if (bind(_Socket_fd, (struct sockaddr*)&_val, sizeof(_val)) < 0)
		throw "Error: bind";
	if (listen(_Socket_fd, 1000) < 0)
		throw "Error: listen";
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
