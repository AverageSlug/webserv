#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "includes.hpp"
# include "Server.hpp"
# include "all_servers.hpp"

class Request
{
	public:
		Request();
		Request(const Request &);
		Request(const std::string content);
		~Request();
		Request&	operator=(const Request &);

		const t_location*	getLocation() const;
		std::string			getMethod();
		int					getStatus() const;
		std::string			getConstructPath();
		bool				setRequestUri(const std::string &);
		void				setConstructPath();
		void				setConstructPath(const std::string &);
		void				setContent();
		void				setHeaderData(const std::string &);
		void				setChunked();
		void				setStatus(int status);
		void				setServer(const Server *, int);
		bool				setFileInfo();
		const Server*		getServ();
		std::string			getUri();
		std::string			getContent();
		bool				getChunked();


		bool				parseFile();

		const std::map<std::string, std::string>&	getFileInfo(void) const;


		typedef std::vector<std::string>							vector_type;
		typedef std::vector<t_location *>							location_type;
		typedef std::pair<std::string, std::string>					pair_type;
		typedef std::map<std::string, std::vector<std::string> >	value_type;

		value_type			getData();

	private:

		std::string			_constructPath;
		const Server		*_server;
	
		std::map<std::string, std::string>			_fileInfo;
		int					_status;

		value_type			_data;
		std::string			_request_method;
		std::string			_uri; // 2nd part of the url
		std::string			_content; // complete request converted to string
		bool				_chunked;	
};

#endif
