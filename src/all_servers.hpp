#ifndef ALL_SERVERS_HPP
# define ALL_SERVERS_HPP

# include "includes.hpp"
# include "Server.hpp"

# define OUTSIDE 0
# define IN_SERVER 1
# define IN_LOCA 2

# define NONE 0
# define WAIT_BRACKS 1
# define IN_BRACKS 2

class all_servers
{
	public:
		typedef Server::data_type                            data_type;
	
		all_servers();
		all_servers(const all_servers &);
		~all_servers();
		all_servers&	operator=(const all_servers &);
		
		size_t				size() const;

		Server*				operator[](int i) const;

		Server*				last() const;

		void	parser(const char *path);
		void	parse_line(std::string str);


		void				newParserLine(Server *, const data_type &);
		void				newLocation(Server *, const data_type &);
		void				openBlock(const data_type &);
		void				closeBlock(const data_type &);
		std::vector<Server *>	getservs() {return _servers;}

		
	private:
		std::vector<Server *>	_servers;
		std::map<int, int>		_bracket;
};

std::ostream&	operator<<(std::ostream &, const all_servers &);

#endif
