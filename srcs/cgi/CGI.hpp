#ifndef CGI_HPP
#define CGI_HPP

#include "Webserv.hpp"
#include "HTTPRequest.hpp"
#include "utils.hpp"
#include "ServerError.hpp"

#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string>
#include <iostream>
#include <sstream>

#define CGI_BUFF_SIZE 8192

class CGI
{
	private:
		/* data */
		DISALLOW_COPY_AND_ASSIGN(CGI);
		void wait_parent(pid_t pid);
		char **init_argv(std::string filename);
		int	_inFd;
		int _outFd;
		int _pipeFd[2];
		char **_argv;
		char **_env;
	public:
		CGI(/* args */);
		~CGI();
		void runCGI(HTTPRequest &request);
		std::string readCGI();
		int getInFd();
		int getOutFd();
		void setInFd(int inFd);
		void setOutFd(int outFd);
		void setEnv(std::string pathinfo, std::string query);
		void setArgv(char **argv);
		void deleteEnv();
		void deleteArgv();
};

#endif
