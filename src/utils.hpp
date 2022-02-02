/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ijacquet <ijacquet@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 13:59:45 by igor              #+#    #+#             */
/*   Updated: 2022/02/02 12:28:42 by ijacquet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef UTILS_HPP
# define UTILS_HPP

# include "includes.hpp"

bool	ft_isNumeric(const std::string);
bool	ft_CheckIP(const std::string);
bool	ft_checkDir(const std::string);
bool	ft_checkPath(const std::string);
std::vector<std::string>	ft_strtovec(const std::string s, const std::string delim);
std::string	getContent(const std::string& file);




#endif