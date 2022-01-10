#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Response_header.hpp"

class Response {
	public:
		Response();
		~Response();
		Response(const Response &cpy);
		Response	&operator=(const Response &a);
		void		call_method();
		void		get_method();
		void		post_method();
		void		delete_method();
	private:
		void	(Response::*_methods[3])(void);
		std::string	_header;
};

#endif