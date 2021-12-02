/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaurids <nlaurids@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/02 12:06:17 by nlaurids          #+#    #+#             */
/*   Updated: 2021/12/02 12:06:17 by nlaurids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
# include "includes.hpp"
# include "all_servers.hpp"

class Socket
{
	public:
		Socket(const all_servers &all_servers, int i);
		Socket();
		~Socket();
		Socket(const Socket &cpy);
		Socket &operator=(const Socket &a);
		int		setup();
		int		getFD();
		std::vector<int>	getConnecting();
		void	new_fd();
	private:
		int		_Socket_fd;
		int		_reuse_addr;
		struct	sockaddr_in	_val;
		std::vector<int>	_connecting;
};

#endif