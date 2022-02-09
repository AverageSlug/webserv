#include "all_servers.hpp"
#include "Webserv.hpp"

int		main(int argc, char **argv)
{
    char const *config_file;
    if (argc == 2)
    {
        config_file = argv[1];
    }
    else if (argc == 1)
    {
        config_file = "src/conf/default.conf";
    }
    else
    {
        std::cout << "error" << std::endl;
        return (1);
    }
    all_servers all_servers;
    try
    {
        all_servers.parser(config_file);
    }
	catch (const char* & e)
	{
		std::cerr << e << std::endl;
		exit(1);
	}
	try
	{
		Webserv Webserv(all_servers);
		Webserv.setup();
		Webserv.server();
	}
	catch(const char* e)
	{
		std::cerr << e << '\n';
	}
}
