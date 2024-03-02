// #pragma once
#ifndef CGI_HPP
#define CGI_HPP

#include "Webserv.hpp"
#include "HTTPRequest.hpp"
#include "QueryParse.hpp"
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

extern char **environ;

class CGI
{
	private:
		/* data */
		DISALLOW_COPY_AND_ASSIGN(CGI);
		std::string input_pipe(int pipeFd);
		void wait_parent(pid_t pid);
		char **init_argv(std::string filename);
		int	_inFd;
		int _outFd;
		enum cgi_mode _mode;
	public:
		CGI(/* args */);
		~CGI();
		std::string runCGI(HTTPRequest &request);
		std::vector<std::string> parse_split_char(std::string uri_argv, char del);
		// std::vector<std::string> parse_split_str(std::string uri_argv, std::string del);
		int getInFd();
		int getOutFd();
		enum cgi_mode getMode();
		void setInFd(int inFd);
		void setOutFd(int outFd);
		void cgiMode(enum cgi_mode mode);
};

#endif