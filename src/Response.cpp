#include "Response.hpp"

Response::Response(Request *request)
{
	_request = request;
}

Response::~Response() {}

Response::Response(const Response &cpy)
{
	*this = cpy;
}

Response &Response::operator=(const Response &a)
{
	(void)a; //toudou
	return (*this);
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
	// _status_codes[100] = "Continue";
	// _status_codes[200] = "OK";
	// _status_codes[201] = "Created";
	// _status_codes[204] = "No Content";
	// _status_codes[400] = "Bad Request";
	// _status_codes[403] = "Forbidden";
	// _status_codes[404] = "Not Found";
	// _status_codes[405] = "Method Not Allowed";
	// _status_codes[413] = "Payload Too Large";
	// _status_codes[500] = "Internal Server Error";
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
	if (_request->getMethod() == "GET")
		ft_get(file_content);
	else if (_request->getMethod() == "POST")
		ft_post();
	else if (_request->getMethod() == "DELETE")
		ft_delete();
//	if (_status.first >= 400)
		//gestion d'erreur
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
//			else
//				content += std::to_string(statStruct.st_size);   !!!!!!!!!
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
		_content = content;
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
	std::map<std::string, std::string>	fileInfo = _request->getFileInfo();

	for (std::map<std::string, std::string>::iterator	it = fileInfo.begin(); it != fileInfo.end(); ++it)
	{
		std::string	toUploadPath = "./all_data" + _request->getLocation()->uploadStore + it->first;
		if (_request->getServ()->clientMaxBodySize() &&
			getFileLength(toUploadPath) > _request->getServ()->clientMaxBodySize())
		{
			setStatus(413);
			return false;
		}
		std::ofstream	ofs;//(toUploadPath, std::ofstream::out); !!!!
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
	_allow = "GET";
	_content_language = "en";
	std::stringstream(_content_length) << _request->getContent().length();
	_content_location = _request->getLocation()->path;
	{} //content-type
	{} //date
	{} //last-modified
	if (_status.first == 201 || (_status.first >= 300 && _status.first <= 308))
	{
		_location = _request->getLocation()->root;
	}
	if (_status.first == 301 || _status.first == 429 || _status.first == 503)
	{
		std::stringstream(_retry_after) << 3;
	}
	_server = "webigornulserv/4.2.0";
	if (_request->getChunked())
		_transfer_encoding = "chunked";
	else
		_transfer_encoding = "identity";
	if (_status.first == 401)
	{
		_www_authenticate = "Basic";
	}
}

std::string	Response::_get_headers()
{
	std::string	header;
	if (_allow != "")
		header += "Allow: " + _allow + "\r\n";
	if (_content_language != "")
		header += "Content-Language: " + _content_language + "\r\n";
	if (_content_length != "")
		header += "Content-Length: " + _content_length + "\r\n";
	if (_content_location != "")
		header += "Content-Location: " + _content_location + "\r\n";
	if (_content_type != "")
		header += "Content-Type: " + _content_type + "\r\n";
	if (_date != "")
		header += "Date: " + _date + "\r\n";
	if (_last_modified != "")
		header += "Last-Modified: " + _last_modified + "\r\n";
	if (_location != "")
		header += "Location: " + _location + "\r\n";
	if (_retry_after != "")
		header += "Retry-After: " + _retry_after + "\r\n";
	if (_server != "")
		header += "Server: " + _server + "\r\n";
	if (_transfer_encoding != "")
		header += "Transfer-Encoding: " + _transfer_encoding + "\r\n";
	if (_www_authenticate != "")
		header += "WWW-Authenticate: " + _www_authenticate + "\r\n";
	return (header);
}

std::string	Response::header()
{
	std::string	header;
	std::string	tmp;
	_init();
	std::string tmp2 = setIndex(_request->getConstructPath());
	_set_headers();
	std::stringstream(tmp) << _status.first;
	header = "HTTP/1.1 " + tmp + _status.second + " \r\n";
	header += _get_headers();
	header += "\r\n" + tmp2;
	return (header);
}

