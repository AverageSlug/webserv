#ifndef RESPONSE_HEADER_HPP
# define RESPONSE_HEADER_HPP

# include "includes.hpp"

class Response_header {
	public:
		Response_header();
		~Response_header();
		Response_header(const Response_header &cpy);
		Response_header		&operator=(const Response_header &a);
		std::string			header();
	private:
		void		_init();
		void		_set_headers(const std::string lang, int size, const std::string location, const std::string type, const std::string path, int status);
		std::string	_get_headers();
		std::string	_allow;
		std::string	_content_language;
		std::string	_content_length;
		std::string	_content_location;
		std::string	_content_type;
		std::string	_date;
		std::string	_last_modified;
		std::string	_location;
		std::string	_retry_after;
		std::string	_server;
		std::string	_transfer_encoding;
		std::string	_www_authenticate;
		std::string	_status_codes[501];
};

#endif