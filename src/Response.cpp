#include "Response.hpp"

Response::Response(Request *request)
{
	setStatus(request->getStatus());
	_request = request;
}

Response::~Response() {}

Response::Response(const Response &cpy)
{
	*this = cpy;
}

Response &Response::operator=(const Response &a)
{
	(void)a; //toudoudou
	return (*this);
}

std::string Response::getContent()
{
	return _content;
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
	_location = "";
	_retry_after = "";
	_server = "";
	_transfer_encoding = "";
	_www_authenticate = "";
}

void	Response::setStatus(int status_code)
{
	if (status_code == 100)
		_status = std::make_pair(status_code, "Continue");
	else if (status_code == 200)
		_status = std::make_pair(status_code, "OK");
	else if (status_code == 201)
		_status = std::make_pair(status_code, "Created");
	else if (status_code == 204)
		_status = std::make_pair(status_code, "No Content");
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
	if (_request->getLocation())
		for (size_t i = 0; i < _request->getLocation()->methods.size(); ++i)
			if (method == _request->getLocation()->methods[i])
				return true;
	setStatus(405);
	return false;
}

void    Response::setContent(const std::string file_content)
{
	if (_status.first < 400)
		checkMethod(_request->getMethod());

	// check cgi to do
	// std::cout << _request->getLocation()[1].cgi.first << std::endl;//comment that I can find if I type this
	// if (ft_checkPath(_request->getLocation()->path))//
	// 	{}//
	if (ft_checkDir(_request->getConstructPath()))
		_content = setIndex(_request->getConstructPath());
	else if (_request->getMethod() == "GET")
		ft_get(file_content);
	else if (_request->getMethod() == "POST")
		ft_post();
	else if (_request->getMethod() == "DELETE")
		ft_delete();
	if (_status.first >= 400)
		setErrorContent();
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
	const Server *lol = _request->getServ();
	std::cout << "set error 1 : status = " << _request->getServ() << std::endl;
	std::cout << lol->name()[0] << std::endl;
	std::cout << "set error 1 : status = " << _status.first << std::endl;
	it = _request->getServ()->errorPages().find(413);
	std::cout << "set error 2\n";
	/* Default error page setup case */
	if (it != _request->getServ()->errorPages().end() &&
		ft_checkPath(it->second))
	{
		_content = getFileContent(it->second);
		return ;
	}
	
	/* Default case */
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
		 
	if (ft_checkDir(_request->getConstructPath())) // if directory
	{
		if (_request->getLocation() && _request->getLocation()->autoindex == true)
			_content = setIndex(_request->getConstructPath());
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
	if (false == _request->setFileInfo())
		return false;

	std::map<std::string, std::string>	fileInfo = _request->getFileInfo();

	for (std::map<std::string, std::string>::iterator	it = fileInfo.begin(); it != fileInfo.end(); ++it)
	{
		std::string	toUploadPath = "./all_data" + _request->getLocation()->uploadStore + it->first;
	// std::cout << "UPLOADFILE PRINT HERE\nTouploadpath :" << toUploadPath << " File length : " << getFileLength(toUploadPath);
		if (_request->getServ()->clientMaxBodySize() &&
			getFileLength(toUploadPath) > _request->getServ()->clientMaxBodySize())
		{
			setStatus(413);
			return false;
		}
		std::ofstream	ofs(toUploadPath, std::ofstream::out);// !!!!
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
	if (_request->getLocation() && !_request->getLocation()->uploadStore.empty())
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
	else if ((size_t)_request->getServ()->clientMaxBodySize() != 0 &&
		_request->getContent().size() > (size_t)_request->getServ()->clientMaxBodySize())
		setStatus(413);
	else
		_content = _request->getContent();
}

void	Response::ft_delete()
{
	if (_status.first != 200)
		return ;
	
	if (std::remove(_request->getConstructPath().c_str()) != 0)
		setStatus(403);
	_content += "<html><body><h1>File deleted.</h1></body></html>";
}

void	Response::_set_headers()
{
//	_allow = _request->getMethod(); //this is supposed to be the list of allowed methods
//	_content_language = "en";
	std::stringstream ss;
	ss << _content.length();
	_content_length = ss.str();
//	_content_location = _request->getUri();
//	_content_type = ""; //content-type !!
//	_date = "Wed, 02 Feb 2022 12:05:59"; //date !!
//	_last_modified = "Mon, 29 Jun 2000"; //last-modified !!
	if (_status.first == 201 || (_status.first >= 300 && _status.first <= 308))
	{
		_location = _request->getLocation()->root;
	}
	if (_status.first == 301 || _status.first == 429 || _status.first == 503)
	{
		std::stringstream(_retry_after) << 3;
	}
//	_server = "webigornulserv/4.2.0";
//	if (_request->getChunked())
//		_transfer_encoding = "chunked";
//	else
//		_transfer_encoding = "identity";
//	if (_status.first == 401)
//	{
//		_www_authenticate = "Basic";
//	}
}

std::string	Response::_get_headers()
{
	 std::string	header;
	// if (_allow != "")
	// 	header += "Allow: " + _allow + "\r\n";
	// if (_content_language != "")
	// 	header += "Content-Language: " + _content_language + "\r\n";
	if (_content_length != "")
		header += "Content-Length: " + _content_length + "\r\n";
	// if (_content_location != "")
	// 	header += "Content-Location: " + _content_location + "\r\n";
	// if (_content_type != "")
	// 	header += "Content-Type: " + _content_type + "\r\n";
	// if (_date != "")
	// 	header += "Date: " + _date + "\r\n";
	// if (_last_modified != "")
	// 	header += "Last-Modified: " + _last_modified + "\r\n";
	// if (_location != "")
	// 	header += "Location: " + _location + "\r\n";
	// if (_retry_after != "")
	// 	header += "Retry-After: " + _retry_after + "\r\n";
	// if (_server != "")
	// 	header += "Server: " + _server + "\r\n";
	// if (_transfer_encoding != "")
	// 	header += "Transfer-Encoding: " + _transfer_encoding + "\r\n";
	// if (_www_authenticate != "")
	// 	header += "WWW-Authenticate: " + _www_authenticate + "\r\n";
	return (header);
}

void	Response::header()
{
	std::string	header;
	//std::string	tmp;
	_init();
	//std::string tmp2 = setIndex(_request->getConstructPath());
	_set_headers();
	//std::stringstream(tmp) << 200;//_status.first;
	header = "HTTP/1.1 ";
	std::stringstream ss;
	ss << _status.first;
	header += ss.str() + " " + _status.second + "\r\n"; //200 OK\r\n";// + _status.second + "\r\n";
	header += _get_headers() + "\r\n";
	//header += "\r\n" + tmp2;
	_header = header;
}

std::string	Response::get_header()
{
	return (_header);
}
