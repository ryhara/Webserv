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
#include "ServerError.hpp"
#include "Client.hpp"
#include "utils.hpp"
#include "Config.hpp"

# define SERVER_PORT			4242
# define SERVER_PORT_STR		"4242"
# define QUEUE_LENGTH			20
# define BUFFER_SIZE			8192

# define DISALLOW_COPY_AND_ASSIGN(ClassName) \
	ClassName(const ClassName &); \
	ClassName &operator=(const ClassName &)

class Server
{
	private:
		std::map<int, std::vector<ServerConfig> > _servers; // port, server_config
		std::map<std::string, int>	_server_fds; // PORT_STR, server_fd
		int							_server_fd;
		std::map<int, Client*>		_clients;
		struct addrinfo _hints;
		struct addrinfo *_res;
		fd_set						_readfds;
		fd_set						_writefds;
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
		void	executeRecvProcess(std::map<int, Client*>::iterator &it);
		void	executeSendProcess(std::map<int, Client*>::iterator &it);
		void	deleteClient(std::map<int, Client*>::iterator &it);
		void	deleteAllClients(void);
		void	mainLoop(void);
		int		acceptSocket(int &server_fd);
		void	clientProcess(int client_fd);
		void	parentProcess(pid_t pid);
		void	closeServerFds(void);
		void	closeClientFds(int client_fd);
		void 	serverEvent(void);
		void	catchError(std::map<int, Client*>::iterator &it, ServerException &e);
		void	CGIEvent(std::map<int, Client*>::iterator &it);
		void	sendCGIResponse(std::map<int, Client*>::iterator &it);
		void	sendEvent(std::map<int, Client*>::iterator &it);
	public:
		Server(void);
		~Server(void);
		void	start(void);
		Config	&getConfig(void) const;
		void	setServers(std::map<int, std::vector<ServerConfig> > &servers);
};
