#include "Response.hpp"

Response::Response()
{
	_location = NULL;
	setStatus(200);
}

Response::Response(Request request) : _request(request)
{
	if (_request.getStatus() == 400)
		_location = NULL;
	else
		_location = request.getLocation();
	setStatus(request.getStatus());
	if (!ft_checkPath(_request.getConstructPath().substr(0, _request.getConstructPath().find("?"))) && _status.first < 400)
		setStatus(404);
}

Response::~Response()
{
}

Response::Response(const Response &cpy)
{
	*this = cpy;
}

Response &Response::operator=(const Response &a)
{
	_request = a._request;
	if (_request.getStatus() == 400)
		_location = NULL;
	else
		_location = a._request.getLocation();
	setStatus(a._request.getStatus());
	return (*this);
}

std::string Response::getContent()
{
	return _content;
}

std::pair<int, std::string> Response::getStatus()
{
	return _status;
}

void	Response::_init()
{
	_allow = "";
	_content_language = "";
	_content_length = "";
	_content_location = "";
	_content_type = "";
	_date = "";
	_last_modified = "";
	_location_path = "";
	_retry_after = "";
	_server = "";
	_transfer_encoding = "";
	_www_authenticate = "";
}

void	Response::setStatus(int status_code)
{
	if (_location && !_location->redirection.second.empty())
	{
		if (ft_checkPath("./all_data" + _location->redirection.second))
			status_code = _location->redirection.first;
		else
			status_code = 404;
	}
	if (status_code == 100)
		_status = std::make_pair(status_code, "Continue");
	else if (status_code == 200)
		_status = std::make_pair(status_code, "OK");
	else if (status_code == 201)
		_status = std::make_pair(status_code, "Created");
	else if (status_code == 204)
		_status = std::make_pair(status_code, "No Content");
	else if (status_code == 301)
		_status = std::make_pair(status_code, "Moved Permanently");
	else if (status_code == 302)
		_status = std::make_pair(status_code, "Found");
	else if (status_code == 303)
		_status = std::make_pair(status_code, "See Other");
	else if (status_code == 304)
		_status = std::make_pair(status_code, "Not Modified");
	else if (status_code == 308)
		_status = std::make_pair(status_code, "Permanent Redirect");
	else if (status_code == 400)
		_status = std::make_pair(status_code, "Bad Request");
	else if (status_code == 403)
		_status = std::make_pair(status_code, "Forbidden");
	else if (status_code == 404)
		_status = std::make_pair(status_code, "Not Found");
	else if (status_code == 405)
		_status = std::make_pair(status_code, "Method Not Allowed");
	else if (status_code == 413)
		_status = std::make_pair(status_code, "Payload Too Large");
	else if (status_code == 500)
		_status = std::make_pair(status_code, "Internal Server Error");
}

bool	Response::checkMethod(const std::string method)
{
	if (method == "GET")
		return true;
	if (_location)
		for (size_t i = 0; i < _location->methods.size(); ++i)
			if (method == _location->methods[i])
				return true;
	setStatus(405);
	return false;
}

void    Response::setContent(const std::string file_content)
{
	if (_status.first < 400)
		checkMethod(_request.getMethod());
	if (_status.first >= 400)
	{
		setErrorContent();
		return ;
	}
	if (!ft_checkDir(_request.getConstructPath()) && _request.getLocation()->cgi.first.length())
	{
		CGI cgi(_request);
		_content = cgi.exec(_request.getLocation()->cgi.second);
	}
	else if (_request.getMethod() == "GET")
		ft_get(file_content);
	else if (_request.getMethod() == "POST")
		ft_post();
	else if (_request.getMethod() == "DELETE")
		ft_delete();
	if (_status.first >= 400)
	{
		setErrorContent();
		return ;
	}
}


const std::string	Response::setIndex(std::string const path) const
{
	std::string		contentBuf;
	std::string		content;
	std::string		modifTime;
	std::string		fileName;
	DIR				*dir = opendir(path.c_str());
	struct stat		statStruct;
	struct dirent	*dirStruct;
	
	content = "<html><body><h1>Directory : ";
	content += path;
	content += "</h1><hr/>";

	content += "<table><tr><th>Name</th><th>Last modified</th><th>size</th></tr>";

	if (!dir)
		return std::string("");

	while ((dirStruct = readdir(dir)) != NULL)
	{
		fileName = dirStruct->d_name;
		if (fileName == ".")
			fileName.clear();
		else
		{
			contentBuf = path;
			contentBuf += "/";
			if (fileName != "..")
				contentBuf += fileName;
			else
				contentBuf = fileName;
			stat(contentBuf.c_str(), &statStruct);
			modifTime = ctime(&statStruct.st_mtime);
			modifTime.erase(modifTime.end() - 1);
			content += "<tr><td><a href=\"";
			content += fileName;
			if (S_ISDIR(statStruct.st_mode))
				content += "/";
			content += "\">";
			content += fileName;
			if (S_ISDIR(statStruct.st_mode))
				content += "/";
			content += "</a></td>\n";

			content += "<td>";
			content += modifTime;
			content += "</td>";

			content += "<td>";
			if (S_ISDIR(statStruct.st_mode))
				content += "--";
			else
			{
				std::stringstream ss;
				ss << statStruct.st_size;
				content += ss.str();
			}
			content += "</td></tr>\n";
			contentBuf.clear();
			fileName.clear();
			modifTime.clear();
		}
	}
	closedir(dir);
	content += "</table>\n";
	content += "</body>\n";
	content += "</html>\n";

	return content;
}


void	Response::setErrorContent()
{
	std::map<int, std::string>::const_iterator	it;
	if (_status.first != 400)
	{
		it = _request.getServ()->errorPages().find(_status.first);
		if (it != _request.getServ()->errorPages().end() &&
			ft_checkPath(it->second))
		{
			_content = getFileContent(it->second);
			return ;
		}
	}

	std::string content = "<!DOCTYPE html>\r\n";
	content += "<html lang=\"en\">\r\n";
	content += "<head>\r\n";
	content += "<meta charset=\"utf-8\" /><meta http-equiv=\"X-UA-Compatible\" ";
	content += "content=\"IE=edge\" /><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\" />\r\n";
	content += "<title>";
	content += _status.second;
	content += " | ";
	std::stringstream ss;
	ss << _status.first;
	content += ss.str();
	content += "</title>\r\n";
	content += "<style type=\"text/css\">";
    content += "body{margin:0}sub{bottom:-.25em}sup{top:-.5em}body,";
	content += "html{width:100%;height:100%;background-color:#21232a}";
	content += "body{color:#fff;text-align:center;text-shadow:0 2px 4px rgba(0,0,0,.5);padding:0;min-height:100%;";
	content += "-webkit-box-shadowinset 0 0 100px rgba(0,0,0,.8);box-shadow:inset 0 0 100px rgba(0,0,0,.8);display:";
	content += "table;font-family:\"Open Sans\",Arial,sans-serif}";
	content += "h1{font-family:inherit;font-weight:500;line-height:1.1;color:inherit;font-size:36px}";
	content += "h1 small{font-size:68%;font-weight:400;line-height:1;color:#777}";
	content += "a{text-decoration:none;color:#fff;font-size:inherit;border-bottom:dotted 1px #707070}";
	content += ".lead{color:silver;font-size:21px;line-height:1.4}";
	content += ".cover{display:table-cell;vertical-align:middle;padding:0 20px}";
	content += "</style>\r\n";
	content += "</head>\r\n";
	content += "<body>\r\n";
	content += "<body>\r\n";
	content += "<div class=\"cover\"><h1>";
	content += _status.second;
	content += " <small>";
	std::stringstream ss2;
	ss2 << _status.first;
	content += ss2.str();
	content += "</small></h1></div>\r\n";
	content += "</body>\r\n";
	
	_content = content;
}

void	Response::ft_get(const std::string content)
{
	 if (_status.first != 200)
	 	return ;
	if (ft_checkDir(_request.getConstructPath())) // if directory
	{
		if (_location && _location->autoindex == true)
			_content = setIndex(_request.getConstructPath());
		else
			setStatus(403);
	}
	else
	{
		_content = content;
	}
}

off_t	Response::getFileLength(std::string file)
{
	struct stat	buf;

	if (ft_checkDir(file))
		return 0;
	if (stat(file.c_str(), &buf) == 0)
		return buf.st_size;
	return -1;
}

bool	Response::uploadFile()
{
	if (false == _request.setFileInfo())
		return false;
	std::map<std::string, std::string>	fileInfo = _request.getFileInfo();

	for (std::map<std::string, std::string>::iterator	it = fileInfo.begin(); it != fileInfo.end(); ++it)
	{
		std::string	toUploadPath = "./all_data" + _location->uploadStore + it->first;
		if (_request.getServ()->clientMaxBodySize() &&
			getFileLength(toUploadPath) > _request.getServ()->clientMaxBodySize())
		{
			setStatus(413);
			return false;
		}
		std::ofstream	ofs(toUploadPath.c_str(), std::ofstream::out);
		if (!ofs.is_open())
		{
			setStatus(403);
			return false;
		}
		ofs << it->second;
		ofs.close();
	}
	return true;
}

void	Response::ft_post()
{
	if (_status.first != 200)
		return ;


	bool isUpload = false;
	/* upload case */
	if (_location && !_location->uploadStore.empty())
	{
		isUpload = uploadFile();
		if (_status.first != 200)
			return ;
	}

	if (isUpload == true)
	{
		_content = "<html>\n";
		_content += "<body>\n";
		_content += "<h1>File uploaded.</h1>\n";
		_content += "</body>\n";
		_content += "</html>\n";
	}
	else if ((size_t)_request.getServ()->clientMaxBodySize() != 0 &&
		_request.getContent().size() > (size_t)_request.getServ()->clientMaxBodySize())
		setStatus(413);
	else
		_content = _request.getContent();
}

void	Response::ft_delete()
{
	if (_status.first != 200)
		return ;
	
	if (std::remove(_request.getConstructPath().c_str()) != 0)
		setStatus(403);
	_content += "<html><body><h1>File deleted.</h1></body></html>";
}

void	Response::_set_headers()
{
	std::stringstream ss;
	if (_status.first != 400 && _request.getLocation()->cgi.first.length())
		ss << _content.substr(_content.find("\r\n\r\n") + 4, _content.length()).length();
	else
		ss << _content.length();
	_content_length = ss.str();
	if (_status.first == 201)
	{
		_location_path = _location->root;
	}
	if (_status.first == 429 || _status.first == 503)
	{
		std::stringstream(_retry_after) << 3;
	}
}

std::string	Response::_get_headers()
{
	std::string	header;
	if (_content_length != "")
		header += "Content-Length: " + _content_length + "\r\n";
	if (_status.first >= 300 && _status.first <= 399)
	{
		header += "Location: ";
		header += _location->redirection.second + "\r\n";
	}
	return (header);
}

void	Response::header()
{
	std::string	header;
	_init();
	_set_headers();
	header = "HTTP/1.1 ";
	std::stringstream ss;
	ss << _status.first;
	header += ss.str() + " " + _status.second + "\r\n";
	header += _get_headers();
	_header = header;
}

std::string	Response::get_header()
{
	return (_header);
}
