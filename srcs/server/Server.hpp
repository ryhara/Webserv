#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <exception>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>

#include "Webserv.hpp"
#include "HTTPRequestParse.hpp"
#include "HTTPResponse.hpp"
#include "utils.hpp"
#include "Config.hpp"

# define SERVER_PORT			4242
# define SERVER_PORT_STR		"4242"
# define QUEUE_LENGTH			5
# define MAX_EVENTS				10
# define BUFFER_SIZE			8192
# define MAX_CLIENTS			6

// TODO : 全てにおいてコピーコンストラクタと代入演算子を禁止する
// TODO : vectorやmapのメモリを開放する
// TODO : 同時にリクエストを行うテストスクリプトを作成する
# define DISALLOW_COPY_AND_ASSIGN(ClassName) \
	ClassName(const ClassName &); \
	ClassName &operator=(const ClassName &)

class Server
{
	private:
		int				_server_fd;
		struct addrinfo _hints;
		struct addrinfo *_res;
		struct pollfd				fds_[MAX_CLIENTS + 1];
		char			_buffer[BUFFER_SIZE];
		HTTPRequest		_request;
		Config			_config;
		DISALLOW_COPY_AND_ASSIGN(Server);

		void	initServerAddr(void);
		void	createSocket(void);
		void	bindSocket(void);
		void	listenSocket(void);
		void	pollInit(void);
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
