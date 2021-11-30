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
		void	server();
	private:
		Server	*_Server;
		int		_server_fd_highest;
		fd_set	_read_fd;
		fd_set	_write_fd;
		void	_fd_set_set();
		void	_handle_fd_set();
};

#endif