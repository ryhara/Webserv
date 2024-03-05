// #pragma once
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
		enum cgi_mode _mode;
		char **_argv;
		char **_cgiEnv;
	public:
		CGI(/* args */);
		~CGI();
		void runCGI(HTTPRequest &request);
		std::string readCGI();
		std::vector<std::string> parse_split_char(std::string uri_argv, char del);
		// std::vector<std::string> parse_split_str(std::string uri_argv, std::string del);
		int getInFd();
		int getOutFd();
		enum cgi_mode getMode();
		void setInFd(int inFd);
		void setOutFd(int outFd);
		void setMode(enum cgi_mode mode);
		void setCgiEnv(std::string pathinfo, std::string query);
		void setArgv(char **argv);
};

#endif
