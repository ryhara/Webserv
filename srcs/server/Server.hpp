#pragma once

#include <iostream>
#include <string>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/event.h>

#include "HTTPRequestParse.hpp"
#include "Color.hpp"
#include "utils.hpp"
#include "Config.hpp"

# define SERVER_PORT			4242
# define SERVER_PORT_STR		"4242"
# define QUEUE_LENGTH			5
# define MAX_EVENTS				10
# define BUFFER_SIZE			8192
# define SERVER_NAME			"localhost"

class Server
{
	private:
		int				_server_fd;
		struct addrinfo _hints;
		struct addrinfo *_res;
		char			_buffer[BUFFER_SIZE];
		HTTPRequest		_request;
		Config			_config;

		void	initServerAddr(void);
		void	createSocket(void);
		void	bindSocket(void);
		void	listenSocket(void);
		void	mainLoop(void);
		int		acceptSocket(void);
		void	childProcess(int client_fd);
		void	parentProcess(pid_t pid);
	public:
		Server(void);
		~Server(void);
		void	start(void);
		Config	&getConfig(void) const;
};
