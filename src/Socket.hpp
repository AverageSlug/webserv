#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "includes.hpp"
# include "all_servers.hpp"

class Socket
{
	public:
		Socket();
		~Socket();
		Socket(const Socket &cpy);
		Socket &operator=(const Socket &a);
		void	setup(const Server &server);
		long	getFD();
		struct	sockaddr_in	&getVal();
	private:
		void	_setaddr(const Server &server);
		long	_Socket_fd;
		struct	sockaddr_in	_val;
};

#endif
