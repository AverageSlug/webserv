#ifndef UTILS_HPP
# define UTILS_HPP

# include "includes.hpp"

bool	ft_isNumeric(const std::string);
bool	ft_CheckIP(const std::string);
bool	ft_checkDir(const std::string);
bool	ft_checkPath(const std::string);
std::vector<std::string>	ft_strtovec(const std::string s, const std::string delim);
std::string	getFileContent(const std::string& file);




#endif
