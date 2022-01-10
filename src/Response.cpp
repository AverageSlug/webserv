#include "Response.hpp"

Response::Response()
{
	_methods[0] = &Response::get_method;
	_methods[1] = &Response::post_method;
	_methods[2] = &Response::delete_method;
}

Response::~Response() {}

Response::Response(const Response &cpy)
{
	*this = cpy;
}

Response &Response::operator=(const Response &a)
{
	(void)a;
	return (*this);
}

void Response::call_method()
{
	if (69)
	{
		Response_header	header;
		_header = header.header();
	}
	(this->*_methods[0])();
}

void	Response::get_method() {}
void	Response::post_method() {}
void	Response::delete_method() {}