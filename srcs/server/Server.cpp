#include "Server.hpp"

Server::Server(void)
{
	ft_memset(&_hints, 0, sizeof(addrinfo));
	_res = NULL;
	ft_memset(_buffer, 0, sizeof(char) * BUFFER_SIZE);
	ft_memset(&fds_, 0, sizeof(struct pollfd) * (MAX_CLIENTS + 1));
}

Server::~Server(void)
{
}

Config &Server::getConfig(void) const
{
	return const_cast<Config &>(_config);
}

// TODO : サーバーを立ち上げる際のエラーはlog_exitで処理する,リクエストやレスポンスのエラーは例外を投げてSTATUS CODEを返す
void Server::initServerAddr(void)
{
	ft_memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, SERVER_PORT_STR, &_hints, &_res) != 0)
		log_exit("getaddrinfo", __LINE__, __FILE__, errno);
}

void Server::createSocket(void)
{
	int option_value = 1; // setsockopt

	_server_fd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	setNonBlockingFd(_server_fd);
	if (_server_fd < 0)
		log_exit("socket", __LINE__, __FILE__, errno);
	if (setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&option_value, sizeof(option_value)) < 0)
	{
		close(_server_fd);
		log_exit("setsockopt", __LINE__, __FILE__, errno);
	}
}

void Server::bindSocket(void)
{
	if (bind(_server_fd, _res->ai_addr, _res->ai_addrlen) < 0)
	{
		close(_server_fd);
		log_exit("bind", __LINE__, __FILE__, errno);
	}
	freeaddrinfo(_res);
}

void Server::listenSocket(void)
{
	if (listen(_server_fd, QUEUE_LENGTH) < 0)
	{
		close(_server_fd);
		log_exit("listen", __LINE__, __FILE__, errno);
	}
}

int Server::acceptSocket(void)
{
	int 				client_fd;
	socklen_t 			client_addr_len;
	struct sockaddr_in	client_addr;

	client_addr_len = sizeof(client_addr);
	client_fd = accept(_server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
	setNonBlockingFd(client_fd);
	if (client_fd < 0)
	{
		close(_server_fd);
		log_exit("accept", __LINE__, __FILE__, errno);
	}
	return client_fd;
}

void Server::childProcess(int client_fd)
{
	// TODO : buffer size調整、第3引数の調べる(MSG_DONTWAIT : ノンブロッキングモードなので使えそう)
	HTTPRequest request;
	HTTPRequestParse request_parse(request);
	ssize_t n = recv(client_fd, _buffer, sizeof(_buffer) - 1, 0);
	if (n < 0)
	{
		close(client_fd);
		close(_server_fd);
		log_exit("recv", __LINE__, __FILE__, errno);
	}
	_buffer[n] = '\0';
	#if DEBUG
		std::cout << "#### [ DEBUG ] request ####" << std::endl << _buffer << std::endl << "##########" << std::endl;
	#endif
	// std::cout << "-- request -- " << std::endl;
	// for (int i = 0; i < n; i++)
	// 	std::cout << _buffer[i];
	// std::cout << "--------------" << std::endl;
	request_parse.parse(_buffer);
	// TODO : responseを正しく実装する （一時的）
	HTTPResponse response;
	response.selectResponse(request);
	std::string responseMessage = response.getResponseMessage();
	#if DEBUG
		std::cout << "##### [ DEBUG ] responseMessage ####" << std::endl << responseMessage << std::endl << "##########" << std::endl;
	#endif
	// std::string responseMessage = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
	ssize_t send_n = send(client_fd, responseMessage.c_str(), responseMessage.size(), 0);
	if (send_n < 0)
	{
		close(client_fd);
		close(_server_fd);
		log_exit("send", __LINE__, __FILE__, errno);
	}
	std::cout << "================= one request end =================" << std::endl;
}

void Server::pollInit(void)
{
	this->fds_[0].fd = this->_server_fd;
	this->fds_[0].events = POLLIN;
	for (int i = 1; i <= MAX_CLIENTS; ++i) {
		this->fds_[i].fd = -1;
		this->fds_[i].events = POLLIN;
	}
}

void Server::mainLoop(void)
{
	int client_fd;

	pollInit();
	for (;;) {
		int	result = poll(fds_, MAX_CLIENTS + 1, -1);
		if (result == -1) {
			log_exit("poll", __LINE__, __FILE__, errno);
		}
		else if (result == 0) {
			// timeout
			continue;
		}
		if (fds_[0].revents & (POLLIN | POLLERR)) {
			client_fd = acceptSocket();
			for (int i = 1; i <= MAX_CLIENTS; ++i) {
				if (fds_[i].fd == -1) {
					fds_[i].fd = client_fd;
					fds_[i].events = POLLIN;
					break;
				}
			}
		}
		for (int i = 1; i <= MAX_CLIENTS; ++i) {
			if (fds_[i].fd != -1 && (fds_[i].revents & (POLLIN | POLLERR))) {
				std::cout << "i : " << i << " / client_fd : " << client_fd << std::endl;
				childProcess(fds_[i].fd);
				close(client_fd);
				fds_[i].fd = -1;
			}
		}
	}
}

void Server::start(void)
{
	std::cout << "================= Server::start =================" << std::endl;
	try {
		// Init server_addr
		initServerAddr();
		// Create socket
		createSocket();
		// TODO : search SIGCHLD, signal(SIGCHLD, SIG_IGN);
		// Bind
		bindSocket();
		// Listen
		listenSocket();
		// Main loop
		mainLoop();
		close(_server_fd);
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

/* example */
// TODO : fcntlを使ったノンブロッキングモードの実装または pollを使った実装
// if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
//     perror("fcntl");
//     close(fd);
//     exit(EXIT_FAILURE);
// }