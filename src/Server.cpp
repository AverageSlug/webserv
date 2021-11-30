#include "Server.hpp"

Server::Server(int f) : _reuse_addr(1)
{
	memset((char *)&_val, 0, sizeof(_val));
	_val.sin_family = AF_INET;
	_val.sin_addr.s_addr = htonl(INADDR_ANY); //
	_val.sin_port = f; //
}

Server::Server() : _reuse_addr(1) {}

Server::~Server() {}

Server::Server(const Server &cpy)
{
	*this = cpy;
}

Server &Server::operator=(const Server &a)
{
	return (*this);
}

void	Server::setup()
{
	if ((_server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &_reuse_addr, sizeof(_reuse_addr)) < 0)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	fcntl(_server_fd, F_SETFL, O_NONBLOCK);
	if (bind(_server_fd, (struct sockaddr*)&_val, sizeof(_val)) < 0)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	if (listen(_server_fd, 10) < 0) //
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	for (int i = 0; i < 10; i++) //
		_connecting[i] = 0;

}

int		Server::getFD()
{
	return (_server_fd);
}

int		Server::getConnecting(int j)
{
	return (_connecting[j]);
}

void	Server::new_fd()
{
	int	connect;
	if ((connect = accept(_server_fd, NULL, NULL)) < 0)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	fcntl(connect, F_SETFL, O_NONBLOCK);
	for (int i = 0; i < 1; i++)
	{
		if (!_connecting[i])
		{
			_connecting[i] = connect;
			connect = -1;
		}
	}
	if (connect != -1)
	{
		std::cout << "error msg" << std::endl;
		close(connect);
	}
}

void	Server::handle_fd(int j)
{
	char buffer[80]; //??
	char *cur_char;

	if (read(_connecting[j], buffer, 80) < 0) //??
	{
		std::cout << "error msg" << std::endl;
		close(_connecting[j]);
		_connecting[j] = 0;
	}
	else
	{
		std::cout << "msg" << std::endl;
		cur_char = buffer;
		while (*cur_char)
		{
			*cur_char = toupper(*cur_char);
			cur_char++;
		}
		std::cout << "msg" << std::endl;
	}
}