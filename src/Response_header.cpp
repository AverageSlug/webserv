#include "Response_header.hpp"

Response_header::Response_header() {}

Response_header::~Response_header() {}

Response_header::Response_header(const Response_header &cpy)
{
	*this = cpy;
}

Response_header &Response_header::operator=(const Response_header &a)
{
	(void)a;
	return (*this);
}

void	Response_header::_init()
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
	_status_codes[100] = "Continue";
	_status_codes[200] = "OK";
	_status_codes[201] = "Created";
	_status_codes[204] = "No Content";
	_status_codes[400] = "Bad Request";
	_status_codes[403] = "Forbidden";
	_status_codes[404] = "Not Found";
	_status_codes[405] = "Method Not Allowed";
	_status_codes[413] = "Payload Too Large";
	_status_codes[500] = "Internal Server Error";
}

void	Response_header::_set_headers(const std::string lang, int size, const std::string location, const std::string type, const std::string path, int status)
{
	_allow = "";
	_content_language = lang;
	std::stringstream(_content_length) << size;
	_content_location = location;
	(void)type;
	{} //content-type
	{} //date
	{} //last-modified
	if (status == 201 || (status >= 300 && status <= 308))
	{
		_location = path;
	}
	if (status == 301 || status == 429 || status == 503)
	{
		std::stringstream(_retry_after) << 3;
	}
	_server = "webigornulserv/4.2.0";
	_transfer_encoding = "identity";
	if (status == 401)
	{
		_www_authenticate = "Basic realm=\"\", charset=\"UTF-8\""; //idk
	}
}

std::string	Response_header::_get_headers()
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

std::string	Response_header::header()
{
	std::string	header;
	std::string	tmp;
	_init();
	//_set_headers(lang, size, location, type, path, status);
	//std::stringstream(tmp) << status;
	//header = "HTTP/1.1 " + tmp + _status_codes[status] + " \r\n";
	header += _get_headers();
	return (header);
}