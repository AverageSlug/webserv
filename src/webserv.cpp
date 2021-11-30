#include "Webserv.hpp"

Webserv::Webserv() {}

Webserv::~Webserv() {}

Webserv::Webserv(const Webserv &cpy)
{
	*this = cpy;
}

Webserv &Webserv::operator=(const Webserv &a)
{
	this->_tmp = a._tmp;
	return (*this);
}

void	Webserv::setup()
{
	_Server = new Server[1]; //
//	while (more config) {
		Server	Server(0); //
		if (Server.setup())
		{
			std::cout << "error" << std::endl;
			exit(1);
		}
		_Server[0] = Server; //
		if (_Server[0].getFD() > _server_fd_highest)
			_server_fd_highest = _Server[0].getFD();
//	}
}

void	Webserv::server()
{
	struct	timeval_t	timeout;
	int		select_fd;

	while (1)
	{
		_fd_set_set();
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if ((select_fd = select(_server_fd_highest + 1, &_read_fd, &_write_fd, NULL, &timeout)) < 0)
		{
			std::cout << "error" << std::endl;
			exit(1);
		}
		if (!select_fd)
			std::cout << "waiting" << std::endl;
		else
			_handle_fd_set();
	}
}

void	Webserv::_fd_set_set()
{
	FD_ZERO(&read_fd);
	FD_ZERO(&write_fd);
	for (int i = 0; _Server[i]; i++)
	{
		FD_SET(_Server[i].getFD(), &read_fd);
		FD_SET(_Server[i].getFD(), &write_fd);
		for (int j = 0; j < 10; j++) //
			if (_Server[i].getConnecting(j))
			{
				FD_SET(_Server[i].getConnecting(j), &read_fd);
				FD_SET(_Server[i].getConnecting(j), &write_fd);
				if (_Server[i].getConnecting(j) > server_fd_highest)
					server_fd_highest = _Server[i].getConnecting(j);
			}
	}
}

void	Webserv::_handle_fd_set()
{
	for (int i = 0; _Server[i]; i++)
	{
		if (FD_ISSET(_Server[i].getFD(), &read_fd) || FD_ISSET(_Server[i].getFD(), &write_fd))
			_Server[i].new_fd();
		for (int j = 0; j < 10; j++)
			if (FD_ISSET(_Server[i].getConnecting(j), &read_fd) || FD_ISSET(_Server[i].getConnecting(j), &write_fd))
				_Server[i].handle_fd(j);
	}
}