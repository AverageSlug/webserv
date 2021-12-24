/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: igor <igor@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 13:59:45 by igor              #+#    #+#             */
/*   Updated: 2021/12/24 10:02:08 by igor             ###   ########.fr       */
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



#endif