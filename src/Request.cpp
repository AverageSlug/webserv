#include "Request.hpp"

// Request::Request() :
// 	_constructPath(ROOT_PATH),
// 	_status(200),
// 	_data(),
// 	_request_method(),
// 	_uri("/"),
// 	_queryString(""),
// 	_content(),
// 	_chunked(false),
// 	_content("")
// {
// }

Request::Request(const std::string content, const all_servers &servs) :
	_constructPath("./all_data"),
	_status(200),
	_data(),
	_request_method(),
	_uri("/"),
	_content(content),
	_chunked(false),
	_all_serv(servs)
{
}

Request&			Request::operator=(const Request &x)
{
	if (this == &x)
		return *this;
	_constructPath = x._constructPath;
	_content = x._content;
	_server = x._server;
	_fileInfo = x._fileInfo;
	_status = x._status;
	_data = x._data;
	_request_method = x._request_method;
	_uri = x._uri;
	_chunked = x._chunked;
	return *this;
}

Request::Request(const Request &x)
{
	*this = x;
}

Request::~Request()
{
}

const Server*	Request::getReqServ(const std::string name) const
{
	for (size_t i = 0; i < _all_serv.size(); ++i)
	{
		for (size_t j = 0; j < _all_serv[i]->name().size(); ++j)
		{
			if (_all_serv[i]->name()[j] == name)
				return _all_serv[i];
		}
	}
	return _all_serv[0];
}

std::string			Request::getMethod()
{
	return _request_method;
}

std::string			Request::getConstructPath()
{
	return _constructPath;
}

const Server*		Request::getServ()
{
	return _server;
}

const std::map<std::string, std::string>&	Request::getFileInfo(void) const
{
	return _fileInfo;
}

std::string			Request::getUri()
{
	return _uri;
}

std::string			Request::getContent()
{
	return _content;
}

bool				Request::getChunked()
{
	return _chunked;
}

Request::value_type		Request::getData()
{
	return _data;
}

int					Request::getStatus()
{
	return _status;
}

const t_location*	Request::getLocation() const
{
	location_type		serv_loc;
	if (_server != NULL)
		serv_loc = _server->locations();
	std::string				path = _uri;
	size_t					pos;

	while (1)
	{
		for (location_type::const_iterator it = serv_loc.begin(); it != serv_loc.end(); ++it)
		{
			if (path == (*it)->path) //!!
			{
				return *it;
			}
			if (path == (*it)->path + "/")
			{
				return *it;
			}
		}
		if (path == "/")
			return NULL;
		if (path.find_last_of('/') == 0)
			path = "/";
		else
		{
			pos = path.substr(0, path.size() - 1).find_last_of('/'); // recup ce qu'il y a avant le last /
			path = path.substr(0, pos + 1);							// qui n'est pas le last char
		}
	}
	return NULL;
}

void	Request::setStatus(int status)
{
	_status = status;
}

bool	Request::setRequestUri(const std::string &first_line)
{
	const vector_type	line = ft_strtovec(first_line, " ");

	if (line.size() != 3)
		return false;
	_request_method = line[0];
	_uri = line[1];
	return true;
}

void	Request::setServer(const Server *server)
{
	_server = server;
}

void	Request::setContent()
{
	std::string					buf(_content);
	const size_t				pos = buf.find("\n");
	
	vector_type					vector_content;
	vector_type::const_iterator	it;
	std::string					tmp_string("");

//	std::cout << "_content before = " << _content << "end of _content before\n";
	if (pos == std::string::npos)
		_content = "";
	else
	{
		buf = buf.substr(pos + 1);
		if (_chunked)
		{
			vector_content = ft_strtovec(buf, "\n");
			it = vector_content.begin();
			if (it != vector_content.end() && (it + 1) != vector_content.end())
			{
				++it;
				for ( ; it != vector_content.end(); it += 2)
				{
					if ((it + 1) == vector_content.end())
						break ;
					tmp_string += *it;
				}
			}
			else
				tmp_string = buf;
		}
		else
			tmp_string = buf;
		_content = tmp_string;
	}
//	std::cout << "_content = " << _content << "end of _content\n";
}

void	Request::setConstructPath()
{
	const t_location					*loc = getLocation();
	std::string							path_tmp(_uri);
	std::string							index_tmp;
	Server::data_type::const_iterator	idx;
	
	if (loc != NULL)
	{
		if (ft_checkDir("./all_data" + path_tmp))
		{
			if (path_tmp[path_tmp.size() - 1] != '/')
				path_tmp += "/";
			for (idx = loc->index.begin(); idx != loc->index.end(); ++idx)
			{
				index_tmp = path_tmp + *idx;
				if (ft_checkPath("./all_data" + index_tmp))
				{
					path_tmp = index_tmp;
					break ;
				}
			}
		}
	}	
	_constructPath += path_tmp;
}

void	Request::setConstructPath(const std::string &path)
{
	_constructPath = path;
}

void	Request::setHeaderData(const std::string& header_str)
{
	const size_t	pos = header_str.find(": ");
	if (pos == std::string::npos)
		return ;

	static pair_type	header_line[] = {
		std::make_pair("Host", ""),
		std::make_pair("Origin", ""),
		std::make_pair("Content-Type", ""),
		std::make_pair("Accept-Encoding", ""),
		std::make_pair("Connection", ""),
		std::make_pair("Upgrade-Insecure-Requests", ""),
		std::make_pair("Accept", ""),
		std::make_pair("Transfer-Encoding", ""),
		std::make_pair("User-Agent", ""),
		std::make_pair("Referer", ""),
		std::make_pair("Accept-Language", "")
	};

	static std::map<std::string, std::string>	
		header_strmap(header_line, header_line + sizeof(header_line) / sizeof(pair_type));
	pair_type	requests_map;
	
	requests_map = std::make_pair(header_str.substr(0, pos), header_str.substr(pos + 2));
	for (std::map<std::string, std::string>::const_iterator opt_it = header_strmap.begin(); opt_it != header_strmap.end(); ++opt_it)
		if (opt_it->first == requests_map.first)
			_data[requests_map.first] = ft_strtovec(requests_map.second, opt_it->second);
}

void	Request::setChunked()
{
	const vector_type			transfert_encoding = _data["Transfer-Encoding"];
	vector_type::const_iterator	it = transfert_encoding.begin();

	_chunked = false;
	for ( ; it != transfert_encoding.end(); ++it)
	{
		if (*it == "chunked")
		{
			_chunked = true;
			break ;
		}
	}
}

bool	Request::setFileInfo()
{
	std::string	toFind[] = {"POST", "multipart/form-data", "boundary", "filename", ""};
	std::string	headerBuf(_content);
	size_t		pos;

	for (size_t i = 0; toFind[i].empty() == false; ++i)
	{
		if ((pos = headerBuf.find(toFind[i])) == std::string::npos)
			return false;
	}
	std::string	boundary = "boundary=";
	std::string buff = headerBuf;
	if ((pos = buff.find(boundary)) != std::string::npos)
	{
		buff.erase(0, pos + boundary.length());
		if ((pos = buff.find("\r\n")) != std::string::npos)
		{
			buff.erase(pos);
			buff.insert(0, "--");
		}
		else
			return false;
	}
	else
		return false;

	std::string toParse = getContent();
	std::string index[] = {"filename=\"", "\"", "Content-Type", "\r\n-------", "\r\n\r\n", buff + "--\r\n"};

	std::string fileName;
	std::string fileContent;

	size_t begin;
	size_t end;

	while (true)
	{
		if ((begin = toParse.find(index[0])) != std::string::npos)
		{
			toParse.erase(0, begin + index[0].length());
		}
		if ((end = toParse.find(index[1])) != std::string::npos)
		{
			fileName = toParse.substr(0, end);
		}
		if ((begin = toParse.find(index[2])) != std::string::npos)
		{
			toParse.erase(0, begin + index[2].length());
		}
		if ((begin = toParse.find(index[4])) != std::string::npos)
		{
			toParse.erase(0, begin + index[4].length());
		}
		if ((end = toParse.find(index[3])) != std::string::npos)
		{
			fileContent = toParse.substr(0, end);
		}
		if (false == fileName.empty())
			_fileInfo.insert(std::make_pair(fileName, fileContent));
		toParse.erase(0, fileContent.length() + index[3].length());
		break;
		if (toParse == index[5])
			break ;
		else
			toParse.erase(0, buff.length() + index[3].length());
	}
	return true;
}

int		Request::reqParser()
{
	vector_type				buffer = ft_strtovec(_content, "\n");
	vector_type::iterator	line = buffer.begin();

	if (setRequestUri(*line++) == false)
	{
		setServer(getReqServ(""));
		return 0;
	}
	for ( ; line != buffer.end() && !(*line).empty(); ++line)
		setHeaderData(*line);
	setServer(getReqServ(_data["Host"][0]));
	setConstructPath();
	setChunked();
//	setContent();
	return 1;
}
