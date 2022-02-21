#include "Server.hpp"

Server::Server() :
	_port(8000),
	_ip("127.0.0.1"),
	_clientMaxBodySize(1000000)
{}

Server::~Server()
{
	for (size_t i = 0; i < _locations.size(); i++)
	 	delete _locations[i];
	_locations.clear();
}

Server::Server(const Server &x)
{
	*this = x;
}

Server&	Server::operator=(const Server &x)
{
	(void)x;
	return *this;
}

const int&	Server::port()	const
{
	return _port;
}

const std::string&	Server::ip()	const
{
	return _ip;
}

const std::vector<std::string>&	Server::name()	const
{
	return _name;
}

const std::map<int, std::string>&	Server::errorPages()	const
{
	return _errorPages;
}

const int&							Server::clientMaxBodySize()	const
{
	return _clientMaxBodySize;
}

const t_location&	Server::locations(int i)	const
{
	return *(_locations[i]);
}

const std::vector<t_location *>&	Server::locations(void)	const
{
	return _locations;
}

size_t	Server::nLoc()	const
{
	return _locations.size();
}

/*
**  Setters
*/

void	Server::setName(const data_type &data) // server_name keyword
{
	_name.assign(data.begin() + 1, data.end());
}

void	Server::setSocket(const data_type &data) // listen keyword
{
	if (data.size() != 2)
		throw "Error while reading configuration file";
	
	size_t pos = data[1].find(':');
	std::string temp_port;
	std::string temp_ip;

	if (pos == std::string::npos)
		temp_port = data[1];
	else
	{
		temp_ip = data[1].substr(0, pos);
		temp_port = data[1].substr(pos + 1);
	}
	if (!ft_CheckIP(temp_ip) || !ft_isNumeric(temp_port))
		throw "Error while reading configuration file";
	std::stringstream(temp_port) >> _port;
	if (_port > 65535 || _port < 1)
		throw "Error while reading configuration file";
	_ip = temp_ip;
}

void	Server::setErrorPages(const data_type &data) // error_page keyword
{
	if (data.size() != 3 || !(ft_isNumeric(data[1])))
		throw "Error while reading configuration file";
	for (data_type::const_iterator it = data.begin() + 1; it != data.end() - 1; it++)
	{
		int error_code;
		if (!ft_isNumeric(*it))
			throw "Error while reading configuration file";
		std::stringstream(*it) >> error_code;
		if (_errorPages.find(error_code) != _errorPages.end())
			_errorPages.erase(error_code);
		_errorPages[error_code] = "./all_data" + std::string("/") + data.back();
	}
}

void	Server::setClientMaxBodySize(const data_type &data) // client_max_body_size keyword
{
	 if (data.size() != 2)
		throw "Error while reading configuration file";
	if (!ft_isNumeric(data[1]))
		throw "Error while reading configuration file";
	std::stringstream(data[1]) >> _clientMaxBodySize;
}

void	Server::setCgi(t_location  *loc, const data_type &data) // cgi_pass keyword
{
	 if (data.size() != 3)
		throw "Error while reading configuration file";
	loc->cgi.first = data[1];
	loc->cgi.second = data[2];
}

void	Server::setIndex(t_location  *loc, const data_type &data) // index keyword
{
	loc->index.assign(data.begin() + 1, data.end());
}

void	Server::setAutoIndex(t_location  *loc, const data_type &data) // autoindex keyword
{
	if (data.size() != 2)
		throw "Error while reading configuration file";
	if (data[1] == "on")
		loc->autoindex = true;
	else if (data[1] != "off")
		throw "Error while reading configuration file";
}

void	Server::setMethod(t_location *loc, const data_type &data) // Allow keyword
{
	for (data_type::const_iterator ite = data.begin() + 1; ite != data.end(); ite++)
	{
		if (*ite == "GET" || *ite == "DELETE" || *ite == "POST")
			loc->methods.push_back(*ite);
		else
			throw "Error while reading configuration file";
	}
}

void	Server::setRedirection(t_location  *loc, const data_type &data) // return keyword
{
	if (data.size() != 3 || !ft_isNumeric(data[1]))
		throw "Error while reading configuration file";
	std::stringstream(data[1]) >> loc->redirection.first;
	if (loc->redirection.first != 307 && loc->redirection.first != 308
		&& (loc->redirection.first < 300 || loc->redirection.first > 304))
		throw "Error while reading configuration file";
	loc->redirection.second = data[2];
}

void	Server::setRoot(t_location  *loc, const data_type &data) // root keyword
{
	if (data.size() != 2)
		throw "Error while reading configuration file";
	if (data[1][0] != '/')
		loc->root = loc->path + data[1];
	else
		loc->root = data[1];
}

void	Server::setUploadStore(t_location  *loc, const data_type &data) // upload_store keyword
{
	if (data.size() != 2 || !ft_checkDir("./all_data" + data[1]))
		throw "Error while reading configuration file";
	loc->uploadStore = data[1];
	if (loc->uploadStore[loc->uploadStore.size() - 1] != '/')
		loc->uploadStore += "/";
}

void	Server::newLocation(const data_type &data)
{
	t_location	*loc;

	if (data.size() != 2 || data[1].find(';') != std::string::npos)
		throw "Error while reading configuration file";
	loc = new t_location;
	loc->path = data[1];
	loc->autoindex = false;
	loc->redirection = std::make_pair(0, "");
	loc->cgi = std::make_pair("", "");
	_locations.push_back(loc);
}

/* Adds the new directive to the location */
void	Server::newLocationDirective(const data_type &data)
{
	std::string				directives[] = {
		"allow",
		"return",
		"root",
		"index",
		"autoindex",
		"cgi_pass",
		"upload_store",
		""
	};
	static method_function1	method_ptr[] = {
		&Server::setMethod,
		&Server::setRedirection,
		&Server::setRoot,
		&Server::setIndex,
		&Server::setAutoIndex,
		&Server::setCgi,
		&Server::setUploadStore
	};
	t_location				*loc = _locations.back();

	if (data.size() < 2)
		throw "Error while reading configuration file";
	for (size_t i = 0; !directives[i].empty(); ++i)
	{
		if (data[0] == directives[i])
		{
			method_function1 func = method_ptr[i];
			(this->*func)(loc, data);
			return ;
		}
	}
	throw "Error while reading configuration file";
}

void	Server::newDirective(const data_type &datas)
{
	if (datas.empty())
		return ;
	
	static std::string		directives[] = {
		"listen",
		"server_name",
		"error_page",
		"client_max_body_size",
		""
	};
	static method_function	method_ptr[] = {
		&Server::setSocket,
		&Server::setName,
		&Server::setErrorPages,
		&Server::setClientMaxBodySize
	};

	for (size_t i = 0; !directives[i].empty(); ++i)
	{
		if (datas[0] == directives[i])
		{
			method_function func = method_ptr[i];
			(this->*func)(datas);
			return ;
		}
	}
	throw "Error while reading configuration file";
}

std::ostream& operator<<(std::ostream& os, const t_location& loc)
{
	os << "\tlocation " << loc.path << std::endl << "\t{" << std::endl;
	if (loc.autoindex == true)
		os << "\t\tautoindex on" << std::endl;
	if (!loc.cgi.first.empty())
		os << "\t\tcgi " << loc.cgi.first << " " << loc.cgi.second << std::endl;
	if (!loc.index.empty())
	{
		os << "\t\tindex";
		for (size_t k = 0; k < loc.index.size(); k++)
			os << " " << loc.index[k];
		os << std::endl;
	}
	if (!loc.redirection.second.empty())
			os << "\t\t" << "return " << loc.redirection.first << " " << loc.redirection.second << std::endl;
	if (!loc.root.empty())
			os << "\t\t" << "root " << loc.root << std::endl;
	if (!loc.uploadStore.empty())
			os << "\t\t" << "upload_store " << loc.uploadStore << std::endl;
	os << "\t}" << std::endl;
	return os;
}

std::ostream& operator<<(std::ostream& os, const Server& server)
{
	os << "server\n" << "{" << std::endl;
	if (server.port() != -1)
		os << "\tlisten " << server.ip() << ":" << server.port() << std::endl;
	if (!server.name().empty())
	{
		os << "\tserver_name";
		for (std::vector<std::string>::const_iterator it = server.name().begin();
			it != server.name().end(); it++)
			os << "  " << *it;
		os << std::endl;
	}
	if (!server.errorPages().empty())
	{
		for (std::map<int, std::string>::const_iterator it = server.errorPages().begin();
			it != server.errorPages().end(); it++)
			os << "\terror_page " << it->first << " " << it->second << std::endl;
	}
	if (server.clientMaxBodySize() != 1000000)
		os << "\tclient_max_body_size " << server.clientMaxBodySize() << std::endl;
	for (size_t j = 0; j < server.nLoc(); j++)
		os << server.locations(j);
	os << "}" << std::endl;
	return os;
}
