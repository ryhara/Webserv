#pragma once

#include <unistd.h>
#include <sys/socket.h>

#include "HTTPRequest.hpp"
#include "HTTPRequestParse.hpp"
#include "HTTPResponse.hpp"
#include "Webserv.hpp"
#include "CGI.hpp"

# define BUFFER_SIZE			8192

enum ClientState
{
	RECV_STATE,
	SEND_STATE,
	CLOSE_STATE,
	CGI_SEND_STATE,
	CGI_READ_STATE
};

class Client
{
	private :
		DISALLOW_COPY_AND_ASSIGN(Client);
		ClientState _state;
		HTTPRequest _request;
		HTTPRequestParse _request_parse;
		HTTPResponse _response;
		int _client_fd;
		int _server_fd;
		static const size_t _buffer_size = BUFFER_SIZE;

	public :
		Client(int client_fd, int server_fd);
		~Client(void);
		HTTPRequestParseState getHTTPRequestParseState(void) const;
		int getClientFd(void) const;
		int getServerFd(void) const;
		bool getKeepAlive(void) const;
		HTTPResponse &getResponse(void);
		HTTPRequest &getRequest(void);
		CGI &getCGI(void);

		const std::string &getResponseMessage(void) const;
		ClientState getState(void) const;
		void setClientFd(int client_fd);
		void setServerFd(int server_fd);
		void setState(ClientState state);

		void clear(void);
		int recvProcess();
		void responseProcess();
		int sendResponse(std::string &responseMessage);
};
