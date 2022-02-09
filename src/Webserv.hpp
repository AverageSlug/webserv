#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "includes.hpp"
# include "Socket.hpp"
# include "Request.hpp"
# include "Response.hpp"

class Webserv
{
	public:
		Webserv(const all_servers &all_servers);
		Webserv();
		~Webserv();
		Webserv(const Webserv &cpy);
		Webserv &operator=(const Webserv &a);
		void	setup();
		void	server();
		size_t	requestLen(std::string const & content);
		all_servers	&getAllServers();
		Socket		*getSocket();
		long		getHighestFD();
		fd_set		getReadFD();
		fd_set		getWriteFD();
		int			reqParser();
		const Server*	getReqServ(const std::string name) const;

	private:
		all_servers	_all_servers;
		Request		*_Request;
		Socket		*_Socket;
		Response	*_Response;
		long	_server_fd_highest;
		fd_set	_set;
		int		_size;
		fd_set	_read_fd;
		fd_set	_write_fd;
		std::map<long, long>	_connecting;
		std::vector<long>		_connected;
		void	_handle_fd_set();
};

#endif
