// #pragma once
#ifndef CGI_HPP
#define CGI_HPP

#include "Webserv.hpp"
#include "../server/HTTPRequest.hpp"//原因説
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "QueryParse.hpp"
#include <string>
#include <iostream>
// #include <sstream>

// extern char **environ;

class Cgi
{
	private:
		/* data */
		DISALLOW_COPY_AND_ASSIGN(Cgi);
		std::string input_pipe(int pipefd);
		void wait_parent(pid_t pid);
		char **init_filename();
		int	infd;
		int outfd;
	public:
		Cgi(/* args */);
		// ~Cgi();
		std::string runCGI(HTTPRequest &request);
		std::vector<std::string> parse_split_char(std::string uri_argv, char del);
		// std::vector<std::string> parse_split_str(std::string uri_argv, std::string del);
		int GetInfd();
		int GetOutfd();
		void setInfd(int infd);
		void setOutfd(int outfd);
};

#endif