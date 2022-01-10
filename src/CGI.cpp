/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlaurids <nlaurids@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 13:09:57 by nlaurids          #+#    #+#             */
/*   Updated: 2021/12/03 13:09:57 by nlaurids         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGI.hpp"

CGI::CGI() {}

/*
CGI::CGI(const Request &request)
{
	_setup(request);
}
*/

CGI::~CGI() {}

CGI::CGI(const CGI &cpy)
{
	*this = cpy;
}

CGI &CGI::operator=(const CGI &a)
{
	_env = a._env;
	return (*this);
}

/*
void	CGI::_setup(const Request &request)
{
	_env["AUTH_TYPE"] = ""; //"" || 'Authorization'
	_env["CONTENT_LENGTH"] = ""; //request get
	_env["CONTENT_TYPE"] = ""; //'Content-Type'
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["PATH_INFO"] = ""; //
	_env["PATH_TRANSLATED"] = ""; //
	_env["QUERY_STRING"] = ""; //
	_env["REMOTE_ADDR"] = ""; //client network address
	_env["REMOTE_HOST"] = ""; //
	_env["REMOTE_IDENT"] = ""; //'Authorization'
	_env["REMOTE_USER"] = ""; //'Authorization'
	_env["REQUEST_METHOD"] = ""; //GET POST DELETE
	_env["SCRIPT_NAME"] = ""; //
	_env["SERVER_NAME"] = ""; //'Host' || REMOTE_ADDR
	_env["SERVER_PORT"] = ""; //
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "webigornulserv"; //custom
}
*/

char		**CGI::_envtoa()
{
	char	**env = new char*[_env.size() + 1];
	if (!env)
		return (NULL);
	int		i = 0;
	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
	{
		std::string tmp = it->first + "=" + it->second;
		env[i] = new char[tmp.size() + 1];
		env[i] = strdup(tmp.c_str());
		i++;
	}
	env[i] = NULL;
	return (env);
}

std::string	CGI::exec(const std::string &script)
{
	std::string	ret;
	pid_t		pid;
	char *const *n;
	char		**env = _envtoa();
	if (!env)
		std::cout << "error or something" << std::endl;
	FILE	*IFile = tmpfile();
	FILE	*OFile = tmpfile();
	long	IFD = fileno(IFile);
	long	OFD = fileno(OFile);
	//
	pid = fork();
	if (pid < 0)
		std::cout << "error or something" << std::endl;
	else if (!pid)
	{
		if (dup2(IFD, 0) < 0)
			std::cout << "error or something" << std::endl;
		if (dup2(OFD, 1) < 0)
			std::cout << "error or something" << std::endl;
		execve(script.c_str(), n, env);
		std::cout << "error or something" << std::endl;
	}
	waitpid(pid, NULL, 0);
	char buff[65536] = {0};
	int read = 1;
	while (read > 0)
	{
		bzero(buff, 65536);
		read = ::read(OFD, buff, 65535);
		ret += buff;
	}
	fclose(IFile);
	fclose(OFile);
	close(IFD);
	close(OFD);
	for (int i = 0; env[i]; i++)
		delete [] env[i];
	delete env;
	return (ret);
}