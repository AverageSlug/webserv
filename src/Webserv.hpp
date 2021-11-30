#ifndef WEBSERV_HPP
# define WEBSERV_HPP
# include "Server.hpp"

class Webserv
{
	public:
		Webserv();
		~Webserv();
		Webserv(const Webserv &cpy);
		Webserv &operator=(const Webserv &a);
		void	setup();
	private:
		Server	*_Server;
		int		_server_fd_highest;
		fd_set	_read_fd;
		fd_set	_write_fd;
};

#endif