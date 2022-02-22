#include "all_servers.hpp"
#include "Webserv.hpp"

#ifdef LEAKS
void	handler(int signal)
{
	(void)signal;
	system("leaks webserv");
}
#endif

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
        std::cout << "error: bad configuration file" << std::endl;
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
	#ifdef LEAKS
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	#endif
	try
	{
		Webserv Webserv;
		Webserv.setup(all_servers);
		Webserv.server(all_servers);
	}
	catch(const char* e)
	{
		std::cerr << e << std::endl;
	}
}
