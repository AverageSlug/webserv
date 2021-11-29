#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>

/* todo:
// use parsed config
// errors
*/

void	fd_set_set(int server_fd, int server_fd_highest, int tmp_listen[1], fd_set read_fd, fd_set write_fd)
{
	FD_ZERO(&read_fd);
	FD_ZERO(&write_fd);
	FD_SET(server_fd, &read_fd);
	FD_SET(server_fd, &write_fd);
	for (int i = 0; i < 1; i++)
	{
		if (tmp_listen[i])
		{
			FD_SET(tmp_listen[i], &read_fd);
			FD_SET(tmp_listen[i], &write_fd);
			if (tmp_listen[i] > server_fd_highest)
				server_fd_highest = tmp_listen[i];
		}
	}
}

void	new_fd(int server_fd, int tmp_listen[1])
{
	int	connect;
	if ((connect = accept(server_fd, NULL, NULL)) < 0)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	fcntl(connect, F_SETFL, O_NONBLOCK);
	for (int i = 0; i < 1; i++)
	{
		if (!tmp_listen[i])
		{
			tmp_listen[i] = connect;
			connect = -1;
		}
	}
	if (connect != -1)
	{
		std::cout << "error msg" << std::endl;
		close(connect);
	}
}

void handle_fd(int tmp_listen)
{
	char buffer[80];
	char *cur_char;

	if (read(tmp_listen, buffer, 80) < 0)
	{
		std::cout << "error msg" << std::endl;
		close(tmp_listen);
		tmp_listen = 0;
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

void	handle_fd_set(int server_fd, int tmp_listen[1], fd_set read_fd, fd_set write_fd)
{
	if (FD_ISSET(server_fd, &read_fd))
		new_fd(server_fd, tmp_listen);
	if (FD_ISSET(server_fd, &write_fd))
		new_fd(server_fd, tmp_listen);
	for (int i = 0; i < 1; i++)
	{
		if (FD_ISSET(tmp_listen[i], &read_fd))
			handle_fd(tmp_listen[i]);
		if (FD_ISSET(tmp_listen[i], &write_fd))
			handle_fd(tmp_listen[i]);
	}
}

int		webserv()
{
	int		server_fd;
	int		server_fd_highest;
	int		reuse_addr = 1;
	struct	sockaddr_in	val;
	int		select_socks;
	fd_set	read_fd;
	fd_set	write_fd;
	struct	timeval		timeout;

//	while (more config) {
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr)) < 0)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	memset((char *)&val, 0, sizeof(val));
	val.sin_family = AF_INET;
	val.sin_addr.s_addr = htonl(INADDR_ANY);
	val.sin_port = 8000; //
	if (bind(server_fd, (struct sockaddr *)&val, sizeof(val)) < 0)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
	if (listen(server_fd, 1) < 0)
	{
		std::cout << "error" << std::endl;
		exit(1);
	}
//	}
	server_fd_highest = server_fd;
	int	tmp_listen[1] = {0};
	while (1) //main
	{
		fd_set_set(server_fd, server_fd_highest, tmp_listen, read_fd, write_fd);
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		if ((select_socks = select(server_fd_highest + 1, &read_fd, &write_fd, NULL, &timeout)) < 0)
		{
			std::cout << "error" << std::endl;
			exit(1);
		}
		if (!select_socks)
			std::cout << "waiting" << std::endl;
		else
			handle_fd_set(server_fd, tmp_listen, read_fd, write_fd);
	}
}

int		main(int argc, char **argv)
{
	(void)argv; //
	if (argc != 2)
	{
		std::cout << "error" << std::endl;
		return (1);
	}
	//parse
	webserv();
}