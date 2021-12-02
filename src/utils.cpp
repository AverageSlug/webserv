#include "includes.hpp"


bool	ft_isNumeric(const std::string &str)
{
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (std::isdigit(str[i]) == 0)
			return false;
	}
	return true;
}