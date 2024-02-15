#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <exception>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>

#include "Webserv.hpp"
#include "HTTPRequestParse.hpp"
#include "HTTPResponse.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "Config.hpp"

# define SERVER_PORT			4242
# define SERVER_PORT_STR		"4242"
# define QUEUE_LENGTH			5
# define BUFFER_SIZE			8192

// TODO : vectorやmapのメモリを開放する

# define DISALLOW_COPY_AND_ASSIGN(ClassName) \
	ClassName(const ClassName &); \
	ClassName &operator=(const ClassName &)

class Server
{
	private:
		// TODO : index, fdにする。index番号のConfigの値を取得したいため
		std::map<int, std::vector<ServerConfig> > _servers;
		std::map<std::string, int>	_server_fds; // PORT_STR, server_fd
		int							_server_fd;
		struct addrinfo _hints;
		struct addrinfo *_res;
		struct pollfd				fds_[FD_SETSIZE];
		fd_set						_readfds;
		std::vector<int>			_client_fds;
		char			_buffer[BUFFER_SIZE];
		HTTPRequest		_request;
		Config			_config;
		DISALLOW_COPY_AND_ASSIGN(Server);

		void setPortAndServerFd(std::string &port, int server_fd);

		void	initServerAddr(std::string &port);
		void	createSocket(std::string &port);
		void	bindSocket(int &server_fd);
		void	listenSocket(int &server_fd);
		void	initFds(void);
		void	mainLoop(void);
		int		acceptSocket(int &server_fd);
		void	clientProcess(int client_fd);
		void	parentProcess(pid_t pid);
		void	closeServerFds(void);
		void 	serverEvent(void);

	public:
		Server(void);
		~Server(void);
		void	start(void);
		Config	&getConfig(void) const;
		void	setServers(std::map<int, std::vector<ServerConfig> > &servers);
};
