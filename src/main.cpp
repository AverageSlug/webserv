/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaurids <nlaurids@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 12:05:42 by nlaurids          #+#    #+#             */
/*   Updated: 2021/12/02 12:05:42 by nlaurids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_servers.hpp"
#include "Webserv.hpp"

int		main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "error" << std::endl;
		return (1);
	}
	all_servers all_servers;
	try
	{
		all_servers.parser(argv[1]);
	}
	catch (const char* & e)
	{
		std::cerr << e << std::endl;
		exit(1);
	}
	Webserv Webserv(all_servers);
	Webserv.setup();
	Webserv.server();
}