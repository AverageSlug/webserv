/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaurids <nlaurids@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 17:16:53 by igor              #+#    #+#             */
/*   Updated: 2022/01/24 13:34:05 by nlaurids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "includes.hpp"

typedef struct s_location
{
	typedef std::pair<int, std::string>                        redirection_type;
	typedef std::pair<std::string, std::string>                cgi_type;

	std::string					path;        // location
	redirection_type			redirection; // HTTP redirection
	std::string					root;        // directory or a file from where the file should be search
	std::vector<std::string>	index;       // default file to answer if the request is a directory
	bool						autoindex;   // turn on or off directory listing
	cgi_type					cgi;         // execute the cgi program
	std::string					uploadStore; // execute the cgi program
	std::vector<std::string>	methods;     // list of accepted HTTP Methods for the root
}	t_location;

class Server
{	private:
		int							_port;              // listen port
		std::string					_ip;                // ip address
		std::vector<std::string>	_name;              // server names
		std::map<int, std::string>	_errorPages;        // default error pages
		int							_clientMaxBodySize; // client max body size (bytes)
		std::vector<t_location *>	_locations;         // routes with rules
	
	public:
		typedef std::vector<std::string>                       data_type;
		Server(void);
		~Server();

		Server(const Server &); //!
		Server&		operator=(const Server &); //!

		const int&							port(void)				const;
		const std::string&					ip(void)				const;
		const std::vector<std::string>&		name(void)				const;
		const std::map<int, std::string>&	errorPages(void)		const;
		const int&							clientMaxBodySize(void)	const;
		const t_location&					locations(int)			const;
		const std::vector<t_location *>&	locations(void)			const;
		size_t								nLoc(void)				const;

		void	setSocket(const data_type &);
		void	setName(const data_type &);
		void	setErrorPages(const data_type &);
		void	setClientMaxBodySize(const data_type &);
		void	setRedirection(t_location *, const data_type &);
		void	setMethod(t_location *, const data_type &);
		void	setRoot(t_location *, const data_type &);
		void	setIndex(t_location *, const data_type &);
		void	setAutoIndex(t_location *, const data_type &);
		void	setCgi(t_location *, const data_type &);
		void	setUploadStore(t_location *, const data_type &);
		void	newLocationDirective(const data_type &);
		
		void	newLocation(const std::vector<std::string> &);
		void	newDirective(const std::vector<std::string> &);

};

std::ostream&	operator<<(std::ostream &, const Server &);
typedef void (Server::*method_function)(const Server::data_type &); //ptr sur fonctions
typedef void (Server::*method_function1)(t_location *, const Server::data_type &);

#endif