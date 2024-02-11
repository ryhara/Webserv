#pragma once

#include <unistd.h>
#include <sys/socket.h>

#include "HTTPRequest.hpp"
#include "HTTPRequestParse.hpp"
#include "HTTPResponse.hpp"
#include "Webserv.hpp"

# define BUFFER_SIZE			8192

class Client
{
	private :
		DISALLOW_COPY_AND_ASSIGN(Client);
		int _client_fd;
		int _server_fd;
		static const size_t _buffer_size = BUFFER_SIZE;
	public :
		Client(int client_fd, int server_fd);
		~Client(void);

		int getClientFd(void) const;
		int getServerFd(void) const;
		void setClientFd(int client_fd);
		void setServerFd(int server_fd);

		int clientProcess();
		int recvProcess(HTTPRequest &request);
		void responseProcess(HTTPRequest &request, HTTPResponse &response);
		int sendResponse(std::string &responseMessage);
};