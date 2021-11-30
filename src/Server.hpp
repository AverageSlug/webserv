#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <cstring>
# include <cstdlib>
# include <unistd.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/time.h>

class Server
{
	public:
		Server(int f);
		Server();
		~Server();
		Server(const Server &cpy);
		Server &operator=(const Server &a);
		void	setup();
		int		getFD();
		int		getConnecting(int j);
		void	new_fd();
		void	handle_fd(int connect);
	private:
		int		_server_fd;
		int		_reuse_addr;
		struct	sockaddr_in	_val;
		int		_connecting[10];
};

#endif