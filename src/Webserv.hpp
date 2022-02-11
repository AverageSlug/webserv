#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include "includes.hpp"
# include "Socket.hpp"
# include "Request.hpp"
# include "Response.hpp"

class Webserv
{
	public:
		Webserv();
		~Webserv();
		Webserv(const Webserv &cpy);
		Webserv &operator=(const Webserv &a);
		void	setup(all_servers &all_servs);
		void	server(all_servers &all_servs);
		size_t	requestLen(std::string const & content);
		int			reqParser(all_servers &all_servs);
		const Server*	getReqServ(const std::string name, all_servers &all_servs) const;

	private:
		Request		_Request;
		Socket		*_Socket;
		Response	_Response;
		long	_server_fd_highest;
		fd_set	_set;
		int		_size;
		fd_set	_read_fd;
		fd_set	_write_fd;
		std::map<long, Server*>	_connecting;
		std::vector<long>		_connected;
		void	_handle_fd_set(all_servers &all_servs);
};

#endif
