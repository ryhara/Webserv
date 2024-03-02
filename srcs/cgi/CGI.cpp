#include "CGI.hpp"

CGI::CGI()
{
	setMode(CGI_START);
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

enum cgi_mode CGI::getMode()
{
	return (_mode);
}

void CGI::setInFd(int inFd)
{
	this -> _inFd = inFd;
}

void CGI::setOutFd(int outFd)
{
	this -> _outFd = outFd;
}

void CGI::setMode(enum cgi_mode mode)
{
	this -> _mode = mode;
}

void	CGI::wait_parent(pid_t pid)
{
	int status;

	ft_sleep(5);
	if (waitpid(pid, &status, WNOHANG) <= 0)
	{
		kill(pid, SIGKILL);
		throw ServerException(STATUS_504, "Gateway Timeout");
	}
}

//filenameの作り方
char	**CGI::init_argv(std::string filename)
{
	char **argv;

	argv = new char *[2];
	argv[0] = const_cast<char *>(filename.c_str());
	argv[1] = NULL;
	return (argv);
}

//ファイルの読み込みについて
std::string    CGI::input_pipe(int fd)
{
    int buf_size=1024;
    char buf[buf_size];
    ssize_t input_size=0;
	std::string new_body = "\0";
    while (1)
    {
        input_size = read(fd, buf, buf_size -1);
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

//ファイル名を受け取るようにするか
//envを取得できるようにし複数のgiファイルの対応をできるようにするか
std::string	CGI::runCGI(HTTPRequest &request)
{
	int pipeFd[2];
	pid_t pid;
	std::string uri = request.getUri();
	std::string query;

	Location location = request.getServerConfig().getLocation(request.getLocation());
	std::string alias = location.getAlias();
	std::string new_uri = uri.substr(location.getLocation().size(), uri.size());
	std::string path = alias + new_uri;
	setArgv(init_argv(path));
	std::string pathinfo;

	// TODO : PATH_INFOにnew_uriを入れる
	// TODO : ?をとる

	size_t found = new_uri.find("?");
	if (found != std::string::npos)
	{
		query = new_uri.substr(found + 1, uri.length());
		pathinfo = new_uri.substr(0, found);
		// std::cout << "before parse : " << uri << std::endl;
		// std::cout << "before parse argv  : " << query << std::endl;
	}
	setCgiEnv(pathinfo, query);
	if (pipe(pipeFd) < 0)
		std::exit(1);
	setNonBlockingFd(pipeFd[0]);
	setNonBlockingFd(pipeFd[1]);
	this -> _inFd = pipeFd[0];
	this -> _outFd = pipeFd[1];
	//TODO :
	pid = fork();
	if (pid < 0)
		log_exit("fork", __LINE__, __FILE__, errno);
	else if (pid == 0)
	{
		// if (close(pipeFd[0]) < 0 || dup2(pipeFd[1], 1) < 0 || close (pipeFd[1]) < 0)
		if (close(pipeFd[0]) < 0 || dup2(pipeFd[1], 1) < 0)
			log_exit("dup2", __LINE__, __FILE__, errno);
		if (access(path.c_str(), X_OK) < 0)
			throw ForbiddenError();
		if (execve(path.c_str(), _argv ,_cgiEnv) < 0)
            log_exit("execve", __LINE__, __FILE__, errno);
	}
	else
    {
        if (close(pipeFd[1]) < 0)
			log_exit("close", __LINE__, __FILE__, errno);
        wait_parent(pid);
		delete [] _argv;
		delete [] _cgiEnv;

    }
	setMode(CGI_READ);
	return (input_pipe(pipeFd[0]));
}

std::vector<std::string> CGI::parse_split_char(std::string uri_argv, char del)
{
	std::vector<std::string> result;
	std::stringstream ss(uri_argv);
	std::string tmp;

	while(std::getline(ss, tmp, del))
	{
		if (!tmp.empty())
			result.push_back(tmp);
	}
	std::cout << "split : " << result[0] << " "<< result[1] << std::endl;
	return (result);
}

// std::vector<std::string> CGI::parse_split_char(std::string uri_argv, std::string del)
// {
// 	std::vector<std::string> result;
// 	std::string::size_type pos = 0;
// 	std::string::size_type before_pos = 0;
// 	std::string::size_type del_size = del.size;

// 	while (1)
// 	{
// 		pos = uri_argv.find(del);
// 		if (pos == std::string::npos)
// 			resurn (result);
// 		result.push_back(uri_argv.substr(0.pos));
// 	}
// }

void	CGI::setCgiEnv(std::string pathinfo, std::string query)
{
	std::string new_pathinfo = "PATH_INFO=";
	std::string new_query = "QUERY_STRING=";

	new_pathinfo += pathinfo;
	new_query += query;
	_cgiEnv = new char*[3];
	_cgiEnv[0] = const_cast<char *>(new_pathinfo.c_str());
	_cgiEnv[1] = const_cast<char *>(new_query.c_str());
	_cgiEnv[2] = NULL;
}


void CGI::setArgv(char **argv)
{
	_argv = argv;
}