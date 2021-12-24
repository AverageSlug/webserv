
#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "includes.hpp"
# include "Server.hpp"
# include "all_servers.hpp"

class Request
{
	public:
		Request(const Request &);
		Request(const std::string content, all_servers servs);
		~Request();
		Request&	operator=(const Request &);

		const t_location*	getLocation() const;
		bool				setRequestUri(const std::string &);
		void				setConstructPath();
		void				setConstructPath(const std::string &);
		void				setContent();
		void				setHeaderData(const std::string &);
		void				setChunked();
		void				setServer(const Server *);
		int					reqParser();
		const Server*		getReqServ(const std::string name) const;


		typedef std::vector<std::string>							vector_type;
		typedef std::vector<t_location *>							location_type;
		typedef std::pair<std::string, std::string>					pair_type;
		typedef std::map<std::string, std::vector<std::string> >	value_type;

	private:

		Request();

		std::string			_constructPath;
		const Server		*_server;
	
		std::map<std::string, std::string>			_fileInfo;
		std::string			_boundary;
		int					_status;

		value_type			_data;
		std::string			_request_method;
		std::string			_uri; // 2nd part of the url
		std::string			_content; // complete request converted to string from `buf'
		bool				_chunked;
		all_servers			_all_serv;
	
};

#endif
