/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaurids <nlaurids@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 13:09:52 by nlaurids          #+#    #+#             */
/*   Updated: 2021/12/03 13:09:52 by nlaurids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include "includes.hpp"
# include "Request.hpp"

class CGI
{
	public:
		CGI();
		~CGI();
		CGI(Request &request);
		CGI(const CGI &cpy);
		CGI &operator=(const CGI &a);
		std::string	exec(const std::string &script);
	private:
		void	_setup(Request &request);
		char	**_envtoa();
		std::string							_req_cont;
		std::map<std::string, std::string>	_env;
};

#endif
