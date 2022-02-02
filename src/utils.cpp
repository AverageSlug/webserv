#include "includes.hpp"


bool	ft_isNumeric(const std::string str)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (std::isdigit(str[i]) == 0)
			return false;
	}
	return true;
}

bool	ft_CheckIP(const std::string ip)
{
	size_t i = 0;
	int j;
	int count = 0;
	std::string tmp;

	while (i < ip.size())
	{
		j = 0;
		tmp = "";
		if (!std::isdigit(ip[i]))
			return false;
		while (std::isdigit(ip[i]))
		{
			tmp = tmp + ip[i];
			i++;
		}
		std::stringstream temp(tmp);
		temp >> j;
		if (j > 255)
			return false;
		count++;
		if ((ip[i] && ip[i] != '.') || count > 4)
			return false;
		i++;
	}
	return true;
}

bool	ft_checkDir(const std::string str)
{
	struct stat info;
	if (stat(str.c_str(), &info) == 0)
	{
		if (S_ISDIR(info.st_mode))
			return (true);
	}
	return (false);
}

bool	ft_checkPath(const std::string str)
{
	std::ifstream test(str.c_str());

	return (test.fail() == 0);
}

std::vector<std::string>	ft_strtovec(const std::string s, const std::string delim)
{
	std::vector<std::string>	vect;
	std::string					tmp(s);
	size_t						pos = tmp.find(delim);

	if (s.empty())
		return vect;
	if (delim[0] != '\0')
	{
		while (pos != std::string::npos)
		{
			vect.push_back(tmp.substr(0, pos));
			tmp.erase(0, pos + delim.length());
			pos = tmp.find(delim);
		}
	}
	if (!tmp.empty())
		vect.push_back(tmp);
	return vect;
}

std::string	getContent(const std::string& file)
{
	std::ifstream	ifs(file, std::ios::in);
	std::string		content;
	std::string		gline;

	if (ifs.is_open())
	{
		while (1)
		{
			std::getline(ifs, gline);
			content += gline;
			if (ifs.eof())
				break ;
			content += "\n";
		}
	}
	ifs.close();
	return content;
}