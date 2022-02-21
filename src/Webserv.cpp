#include "Webserv.hpp"

Webserv::Webserv()
{
}

Webserv::~Webserv()
{
	delete [] _Socket;
}

Webserv::Webserv(const Webserv &cpy)
{
	*this = cpy;
}

Webserv &Webserv::operator=(const Webserv &a)
{
	(void)a;
	return (*this);
}

void	Webserv::setup(all_servers &all_servs)
{
	FD_ZERO(&_set);
	_size = all_servs.size();
	_server_fd_highest = 0;
	_Socket = new Socket[_size];
	for (int i = 0; i < _size; i++)
	{
		_Socket[i].setup(*all_servs.getservs()[i]);
		FD_SET(_Socket[i].getFD(), &_set);
		if (_Socket[i].getFD() > _server_fd_highest)
			_server_fd_highest = _Socket[i].getFD();
	}
}

void	Webserv::server(all_servers &all_servs)
{
	struct	timeval	timeout;
	int		select_fd;

	while (1)
	{
		memcpy(&_read_fd, &_set, sizeof(_set));
		FD_ZERO(&_write_fd);
		for (std::vector<long>::iterator it = _connected.begin(); it < _connected.end(); it++)
			FD_SET(*it, &_write_fd);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		if ((select_fd = select(_server_fd_highest + 1, &_read_fd, &_write_fd, NULL, &timeout)) < 0)
			throw "Error: select";
		if (!select_fd)
			std::cout << "\r" << std::flush;
		else
			_handle_fd_set(all_servs);
	}
}

size_t	Webserv::requestLen(std::string const & content)
{
	size_t pos = content.find("Content-Length: ");

	if (pos == std::string::npos)
		return std::string::npos;
	
	pos += std::string("Content-Length: ").length();

	std::string getLen;

	for (size_t i = pos; i < content.length() - pos; i++)
	{
		if (std::isdigit(content[i]))
			getLen += content[i];
		else
			break ;
	}

	std::stringstream sstream(getLen);
	size_t ret = 0;

	sstream >> ret;
	return ret;
}

const Server*	Webserv::getReqServ(const std::string name, all_servers &all_servs) const
{
	size_t pos = name.find(':');
	std::string temp_port;
	int port;

	if (pos == std::string::npos)
		temp_port = name;
	else
		temp_port = name.substr(pos + 1);
	std::stringstream(temp_port) >> port;
	for (size_t i = 0; i < all_servs.size(); ++i)
	{
		if (all_servs[i]->port() == port)
			return all_servs[i];
	}
	return all_servs[0];
}

int		Webserv::reqParser(all_servers &all_servs)
{
	std::vector<std::string>				buffer = ft_strtovec(_Request.getContent(), "\r\n");
	std::vector<std::string>::iterator		line = buffer.begin();

	if (_Request.setRequestUri(*line++) == false)
	{
		_Request.setServer(getReqServ("", all_servs), _Request.getStatus());
		return 0;
	}
	for ( ; line != buffer.end() && !(*line).empty(); ++line)
		_Request.setHeaderData(*line);
	_Request.setServer(getReqServ(_Request.getData()["Host"][0], all_servs), _Request.getStatus());
	_Request.setConstructPath();
	_Request.setChunked();
	return 1;
}


void	Webserv::_handle_fd_set(all_servers &all_servs)
{
	int v = 1;
	std::string to_send;
	
	for (std::vector<long>::iterator it = _connected.begin(); it != _connected.end(); it++)
	{
		if (FD_ISSET(*it, &_write_fd))
		{
			int sendret = 0;
			Response resp = Response(_Request);
			resp.setContent(getFileContent(_Request.getConstructPath()));
			resp.header();
			to_send = resp.get_header();
			if (resp.getStatus().first == 400 || !_Request.getLocation()->cgi.first.length() || ft_checkDir(_Request.getConstructPath()))
				to_send += "\r\n";
			to_send += resp.getContent();
			std::cout << "Method " << _Request.getMethod() << " : " << _Request.getConstructPath() << " sent with status code " << resp.getStatus().first << " " << resp.getStatus().second << std::endl << std::endl;
			signal(SIGPIPE, SIG_IGN);
			if ((sendret = send(*it, to_send.c_str(), to_send.length(), 0)) <= 0)
			{
				FD_CLR(*it, &_set);
				FD_CLR(*it, &_read_fd);
				_connecting.erase(*it);
			}
			_connected.erase(it);
			v = 0;
			break ;
		}
	}
	for (std::map<long, Server*>::iterator it = _connecting.begin(); v && it != _connecting.end(); it++)
	{
		int ret = 0;
		long sock = it->first;
		if (FD_ISSET(sock, &_read_fd))
		{
			int BUFF_SIZE = it->second->clientMaxBodySize();
			if (it->second->clientMaxBodySize() == 0)
				BUFF_SIZE = 393215;
			char *buff = new char[BUFF_SIZE + 1];
			std::memset(buff, 0, BUFF_SIZE);
			ret = recv(sock, buff, BUFF_SIZE - 1, 0);
			if (ret <= 6)
			{
				if (sock > 0)
				{
					close(sock);
				}
				FD_CLR(sock, &_set);
				FD_CLR(sock, &_read_fd);
				_connecting.erase(sock);
				it = _connecting.begin();
				delete [] buff;
				break ;
			}
			_Request = Request(std::string(buff));
			const int reqLen = requestLen(_Request.getContent());
			if (!std::string(buff).compare(0, 4, "POST") && ((BUFF_SIZE && ret >= 393216) || (reqLen >= BUFF_SIZE && (size_t)reqLen != std::string::npos) || (ret >= BUFF_SIZE - 1)))
			{
				_Request.setStatus(413);
			}
			if ((std::string(buff).compare(0, 3, "GET") && std::string(buff).compare(0, 4, "POST") && std::string(buff).compare(0, 6, "DELETE") && std::string(buff).compare(0, 4, "----")) || reqParser(all_servs) == 0)
				_Request.setStatus(400);
			_connected.push_back(sock);
			delete [] buff;

			break ;
		}
	}
	for (unsigned int i = 0; v && i < all_servs.size(); i++)
	{
		if (FD_ISSET(_Socket[i].getFD(), &_read_fd))
		{
			long	new_socket = accept(_Socket[i].getFD(), NULL, NULL);
			if (new_socket < 0)
				throw "Error: accept";
			fcntl(new_socket, F_SETFL, O_NONBLOCK);
			FD_SET(new_socket, &_set);
			_connecting.insert(std::make_pair(new_socket, all_servs[i]));
			if (new_socket > _server_fd_highest)
				_server_fd_highest = new_socket;
			break ;
		}
	}
}
