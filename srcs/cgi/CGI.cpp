#include "CGI.hpp"

CGI::CGI() : _argv(NULL), _env(NULL)
{
}

CGI::~CGI()
{
}

int	CGI::getInFd()
{
	return (_inFd);
}

int CGI::getOutFd()
{
	return (_outFd);
}

void CGI::setInFd(int inFd)
{
	this -> _inFd = inFd;
}

void CGI::setOutFd(int outFd)
{
	this -> _outFd = outFd;
}

void CGI::setArgv(char **argv)
{
	_argv = argv;
}

void	CGI::setEnv(std::string pathinfo, std::string query)
{
	std::string new_pathinfo = "PATH_INFO=";
	std::string new_query = "QUERY_STRING=";

	new_pathinfo += pathinfo;
	new_query += query;
	_env = new char*[3];
	_env[0] = new char[new_pathinfo.size() + 1];
	std::strcpy(_env[0], new_pathinfo.c_str());
	_env[1] = new char[new_query.size() + 1];
	std::strcpy(_env[1], new_query.c_str());
	_env[2] = NULL;
}

char	**CGI::init_argv(std::string filename)
{
	char **argv;

	argv = new char *[2];
	argv[0] = new char[filename.size() + 1];
	std::strcpy(argv[0], filename.c_str());
	argv[1] = NULL;
	return (argv);
}

void CGI::deleteArgv()
{
	for (int i = 0; _argv[i] != NULL; i++)
		delete [] _argv[i];
	delete [] _argv;
}

void CGI::deleteEnv()
{
	for (int i = 0; _env[i] != NULL; i++)
		delete [] _env[i];
	delete [] _env;
}


void	CGI::wait_parent(pid_t pid)
{
	int status;
	int return_pid;

	ft_sleep(2);
	return_pid = waitpid(pid, &status, WNOHANG);
	if (return_pid == 0)
	{
		kill(pid, SIGKILL);
		if (waitpid(pid, &status, 0) < 0) {
			deleteArgv(); deleteEnv();
			log_exit("waitpid", __LINE__, __FILE__, errno);
		}
		deleteArgv(); deleteEnv();
		throw ServerException(STATUS_504, "Gateway Timeout");
	} else if (return_pid < 0) {
		deleteArgv(); deleteEnv();
		log_exit("waitpid", __LINE__, __FILE__, errno);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
		deleteArgv(); deleteEnv();
		throw ServerException(STATUS_500, "Internal Server Error");
	}
}

std::string	CGI::readCGI()
{
	int fd = _inFd;
	char buf[CGI_BUFF_SIZE];
	ssize_t input_size = 0;
	std::string new_body = "";

	while (1) {
		input_size = read(fd, buf, CGI_BUFF_SIZE - 1);
		if (input_size < 0)
			log_exit("read", __LINE__, __FILE__, errno);
		if (input_size == 0)
			break ;
		buf[input_size] = '\0';
		new_body += static_cast<std::string>(buf);
	}
	close (fd);
	return (new_body);
}

void	CGI::runCGI(HTTPRequest &request)
{
	pid_t pid;
	std::string query = "";
	std::string pathinfo = "";

	std::string uri = request.getUri();
	Location location = request.getServerConfig().getLocation(request.getLocation());
	std::string alias = location.getAlias();
	std::string new_uri = uri.substr(location.getLocation().size(), uri.size());
	std::string path = alias + new_uri;
	setArgv(init_argv(path));

	if (request.getMethod().compare("GET") == 0) {
		pathinfo = uri;
		size_t found = uri.find("?");
		if (found != std::string::npos)
		{
			query = uri.substr(found + 1, uri.length());
			pathinfo = uri.substr(0, found);
		}
		setEnv(pathinfo, query);
		found = new_uri.find("?");
		if (found != std::string::npos) {
			std::string exec_path = new_uri.substr(0, found);
			path = alias + exec_path;
		}
	}
	else if (request.getMethod().compare("POST") == 0)
		setEnv(uri, request.getBody());
	if (pipe(_pipeFd) < 0) {
		deleteArgv(); deleteEnv();
		log_exit("pipe", __LINE__, __FILE__, errno);
	}
	setNonBlockingFd(_pipeFd[0]);
	setNonBlockingFd(_pipeFd[1]);
	setInFd(_pipeFd[0]);
	setOutFd(_pipeFd[1]);

	pid = fork();
	if (pid < 0) {
		deleteArgv(); deleteEnv();
		log_exit("fork", __LINE__, __FILE__, errno);
	}
	else if (pid == 0) {
		if (close(_pipeFd[0]) < 0 || dup2(_pipeFd[1], 1) < 0) {
			deleteArgv(); deleteEnv();
			log_exit("close or dup2", __LINE__, __FILE__, errno);
		}
		if (access(path.c_str(), X_OK) < 0) {
			deleteArgv(); deleteEnv();
			throw ServerException(STATUS_404, "Not Found");
		}
		if (execve(path.c_str(), _argv ,_env) < 0) {
			deleteArgv(); deleteEnv();
			log_exit("execve", __LINE__, __FILE__, errno);
		}
		deleteArgv(); deleteEnv();
	}
	else {
		if (close(_pipeFd[1]) < 0) {
			deleteArgv(); deleteEnv();
			log_exit("close", __LINE__, __FILE__, errno);
		}
		wait_parent(pid);
		deleteArgv();
		deleteEnv();
	}
}
