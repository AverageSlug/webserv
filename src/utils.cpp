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