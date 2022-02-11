#include "CGI.hpp"

CGI::CGI() {}

CGI::CGI(Request &request) :
_req_cont(request.getContent())
{
	_setup(request);
}

CGI::~CGI()
{
}

CGI::CGI(const CGI &cpy)
{
	*this = cpy;
}

CGI &CGI::operator=(const CGI &a)
{
	_env = a._env;
	return (*this);
}

void	CGI::_setup(Request &request)
{
	_env["REDIRECT_STATUS"] = "200";
	_env["AUTH_TYPE"] = ""; //request.getData()["Authorization"][0]; !?
	_env["CONTENT_LENGTH"] = request.getContent().length();
	_env["CONTENT_TYPE"] = ""; //request.getData()["Content-Type"][0];
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	_env["PATH_INFO"] = request.getConstructPath().substr(0, request.getConstructPath().find("?"));
	_env["PATH_TRANSLATED"] = request.getConstructPath().substr(0, request.getConstructPath().find("?"));
	_env["QUERY_STRING"] = request.getUri().substr(request.getUri().find("?") + 1, request.getUri().length());
	_env["REMOTE_ADDR"] = ""; //where do I get user data???
	_env["REMOTE_HOST"] = ""; //where do I get user data???
	_env["REMOTE_IDENT"] = ""; //request.getData()["Authorization"][0]; !?
	_env["REMOTE_USER"] = ""; //request.getData()["Authorization"][0]; !?
	_env["REQUEST_METHOD"] = request.getMethod();
	_env["SCRIPT_NAME"] = request.getLocation()->cgi.second;
	_env["SERVER_NAME"] = request.getData()["Host"][0].substr(0, request.getData()["Host"][0].find(":"));
	_env["SERVER_PORT"] = request.getData()["Host"][0].substr(request.getData()["Host"][0].find(":") + 1, request.getData()["Host"][0].length());
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = request.getServ()->name()[0];
}

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
	char *const *n = NULL;
	char		**env = _envtoa();
	if (!env)
		throw "Error: CGI";
	int		in = dup(0);
	int		out = dup(1);
	FILE	*IFile = tmpfile();
	FILE	*OFile = tmpfile();
	long	IFD = fileno(IFile);
	long	OFD = fileno(OFile);

	write(IFD, _req_cont.c_str(), _req_cont.size());
	lseek(IFD, 0, 0);
	pid = fork();
	if (pid < 0)
		throw "Error: CGI";
	else if (!pid)
	{
		if (dup2(IFD, 0) < 0)
			throw "Error: CGI";
		if (dup2(OFD, 1) < 0)
			throw "Error: CGI";
		execve(script.c_str(), n, env);
		throw "Error: CGI";
		exit(0);
	}
	waitpid(-1, NULL, 0);
	lseek(OFD, 0, 0);
	char buff[65536] = {0};
	int read = 1;
	while (read > 0)
	{
		bzero(buff, 65536);
		read = ::read(OFD, buff, 65536);
		ret += buff;
	}
	dup2(in, 0);
	dup2(out, 1);
	fclose(IFile);
	fclose(OFile);
	close(IFD);
	close(OFD);
	close(in);
	close(out);
	for (int i = 0; env[i]; i++)
		delete [] env[i];
	delete env;
	return (ret);
}
