#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "includes.hpp"
# include "Request.hpp"
# include "CGI.hpp"

class Response
{
	public:
		Response();
		Response(Request req);
		~Response();
		Response(const Response &cpy);
		Response		&operator=(const Response &a);
		void				header();
		void				setStatus(int status_code);
		void				setContent(const std::string file_content);
		void				ft_get(const std::string content);
		void				ft_post();
		void				ft_delete();
		bool				checkMethod(const std::string method);
		const std::string	setIndex(std::string const path) const;
		bool				uploadFile();
		off_t				getFileLength(std::string file);
		std::string			get_header();
		std::string 		getContent();
		
		
		std::pair<int, std::string> getStatus();

		void				setErrorContent();



	private:
		void		_init();
		void		_set_headers();
		std::string	_get_headers();
		std::string	_allow;
		std::string	_content_language;
		std::string	_content_length;
		std::string	_content_location;
		std::string	_content_type;
		std::string	_date;
		std::string	_last_modified;
		std::string	_location_path;
		std::string	_retry_after;
		std::string	_server;
		std::string	_transfer_encoding;
		std::string	_www_authenticate;
		std::pair<int, std::string>	_status;
		Request		_request;
		std::string _content;
		std::string	_header;
		const t_location	*_location;
};

#endif
