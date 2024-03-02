#include "CGI.hpp"

Cgi::Cgi()
{
	infd = 0;
	outfd = 1;
	std::cout << "Cgi constructor called" << std::endl;
}

int	Cgi::GetInfd()
{
	return (infd);
}

int Cgi::GetOutfd()
{
	return (outfd);
}

void Cgi::setInfd(int infd)
{
	this -> infd = infd;
}

void Cgi::setOutfd(int outfd)
{
	this -> outfd = outfd;
}

void	Cgi::wait_parent(pid_t pid)
{
	int status;

	sleep(1);
	if (waitpid(pid, &status, WNOHANG) < 0)
	{
		kill(pid, SIGKILL);
		std::exit(1);
	}
}

//filenameの作り方
char	**Cgi::init_filename()
{
	char **filename;
	std::string file = "test.sh";

	filename = new char *[2];
	filename[0] = const_cast<char *>(file.c_str());
	filename[1] = NULL;
	return (filename);
}

//ファイルの読み込みについて
std::string    Cgi::input_pipe(int fd)
{
    int buf_size=1024;
    char buf[buf_size];
    ssize_t input_size=0;
	std::string new_body = "\0";
    while (1)
    {
		// write(2, "a\n", 2);
        input_size = read(fd, buf, buf_size -1);
        if (input_size < 0)
            std::exit(1);
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
std::string	Cgi::runCGI(HTTPRequest &request)
{
	int pipefd[2];
	pid_t pid;
	char **filename = init_filename();
	std::string uri = request.getUri();
	std::string argv;

	// std::cout << "getbody : "<<request.getBody() << std::endl;
    std::string path = "./www";//uriに入ってない？？
	size_t found = uri.find("?");
	if (found != std::string::npos)
	{
		argv = uri.substr(found + 1, uri.length());
		uri = uri.substr(0, found);
		std::cout << "before parse : " << uri << std::endl;
		std::cout << "before parse argv  : " << argv << std::endl;
		parse_split_char(argv, '&');
	}
    path += uri;
	// std::cout << path << std::endl;
	// std::cout << filename[0] << "\0" << filename[1] << std::endl;
	if (pipe(pipefd) < 0)
		std::exit(1);
	this -> infd = pipefd[0];
	this -> outfd = pipefd[1];
	//ここで一旦切ってfdを返す。selectのところで追加できるようにする。
	pid = fork();
	// if (pid < 0)
	// 	std::exit(1);
	if (pid == 0)
	{
		if (close(pipefd[0]) < 0 ||dup2(pipefd[1], 1) < 0 || close (pipefd[1]) < 0)
			std::exit(1);
		if (access(path.c_str(), X_OK) < 0)
			std::exit(1);
		if (execve(path.c_str(), filename, environ) < 0)
            std::exit(1);
	}
	else
    {
        if (close(pipefd[1]) < 0)
            std::exit(1);
		// std::cerr << "error" << std::endl;
        wait_parent(pid);
		delete [] filename;
		// write(1, "test\n", 5);
    }
	return (input_pipe(pipefd[0]));
}

std::vector<std::string> Cgi::parse_split_char(std::string uri_argv, char del)
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

// std::vector<std::string> Cgi::parse_split_char(std::string uri_argv, std::string del)
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
