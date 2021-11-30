#include "Webserv.hpp"

int		main(int argc, char **argv)
{
	if (argc > 2)
	{
		std::cout << "error" << std::endl;
		return (1);
	}
	Webserv Webserv;
	Webserv.setup();
	Webserv.server();
}