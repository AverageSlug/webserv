#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "includes.hpp"
# include "all_servers.hpp"

class Socket
{
	public:
		void	set_Socket(const Server &server);
		Socket();
		~Socket();
		Socket(const Socket &cpy);
		Socket &operator=(const Socket &a);
		int		setup();
		long	getFD();
		Server	getserv();
		struct	sockaddr_in	&getVal();
	private:
		void	_setaddr();
		long	_Socket_fd;
		Server	_server;
		struct	sockaddr_in	_val;
};

#endif
