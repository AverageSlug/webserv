/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaurids <nlaurids@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 20:40:33 by igor              #+#    #+#             */
/*   Updated: 2021/12/02 13:08:39 by nlaurids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_servers.hpp"
#include "includes.hpp"

static Server::data_type	getdata(std::string str)
{
	Server::data_type data;

	size_t pos = str.find(' ');
	if (pos > str.find('\t'))
		pos = str.find('\t');
	while (pos != std::string::npos)
	{
		std::string newstr = str.substr(0, pos);
		if (!newstr.empty())
			data.push_back(newstr);
		str.erase(0, pos + 1);
		pos = str.find(' ');
		if (pos > str.find('\t'))
			pos = str.find('\t');
	}
	if (str[0])
		data.push_back(str);
	return data;
}

std::string    ft_strcut(const std::string& str, char cut_char)
{
	size_t	pos = str.find(cut_char);

	return str.substr(0, pos);
}

std::vector<std::string>	ft_vectorcut(const std::vector<std::string>& vect, char cut_char)
{
	std::vector<std::string>					newVect(vect);
	std::vector<std::string>::const_iterator	it;
	size_t										pos = 0;

	for(it = newVect.begin(); it != newVect.end(); ++it, ++pos)
	{
		if (*it != ft_strcut(*it, cut_char))
		{
			newVect[pos] = ft_strcut(*it, cut_char);
			if (!newVect[pos].empty())
				++it;
			break ;
		}
	}
	newVect.erase(it, newVect.end());
	return newVect;
}

void	all_servers::parse_line(std::string str)
{
	size_t	pos = str.find('#');

	str = str.substr(0, pos);
	Server::data_type data = getdata(str);
	if (!data.empty())
	{
		if (data[0] == "server")
		{
			if (_bracket[IN_SERVER] != NONE || _bracket[OUTSIDE] != WAIT_BRACKS)
				throw "Error while reading configuration file";
			_servers.push_back(new Server);
			_bracket[IN_SERVER] = WAIT_BRACKS;
		}
		else if (data[0] == "location")
		{
			if (_bracket[IN_LOCA] != NONE || _bracket[IN_SERVER] != IN_BRACKS)
				throw "Error while reading configuration file";
			last()->newLocation(data);
			_bracket[IN_LOCA] = WAIT_BRACKS;
		}
		else if (data[0] == "{")
			openBlock(data);
		else if (data[0] == "}")
			closeBlock(data);
		else
		{
			data = ft_vectorcut(data, ';');
			if (size() == 0)
				throw "Error while reading configuration file";
			newParserLine(last(), data);
		}
	}
}


void	all_servers::parser(const std::string &path)
{
	std::fstream	config_file;
	std::string		str;

	//check l'extension du file
	config_file.open(path);
	if (!config_file)
		throw "open error";
	while (std::getline(config_file, str)) //gnl du fichier de config
		parse_line(str);
	config_file.close();
}