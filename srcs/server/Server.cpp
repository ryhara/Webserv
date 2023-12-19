#include "Server.hpp"

Server::Server(void)
{
	ft_memset(&_hints, 0, sizeof(_hints));
	_res = NULL;
	ft_memset(_buffer, 0, sizeof(_buffer));
}

Server::~Server(void)
{
}

void Server::initServerAddr(void)
{
	ft_memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, "4242", &_hints, &_res) != 0)
		log_exit("getaddrinfo", __LINE__, __FILE__);
}

void Server::createSocket(void)
{
	int option_value = 1; // setsockopt

	_server_fd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	if (_server_fd < 0)
		log_exit("socket", __LINE__, __FILE__);
	setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&option_value, sizeof(option_value));
}

void Server::bindSocket(void)
{
	if (bind(_server_fd, _res->ai_addr, _res->ai_addrlen) < 0)
	{
		close(_server_fd);
		log_exit("bind", __LINE__, __FILE__);
	}
	freeaddrinfo(_res);
}

void Server::listenSocket(void)
{
	if (listen(_server_fd, MAX_CLIENTS) < 0)
	{
		close(_server_fd);
		log_exit("listen", __LINE__, __FILE__);
	}
}

int Server::acceptSocket(void)
{
	int 				client_fd;
	socklen_t 			client_addr_len;
	struct sockaddr_in	client_addr;

	client_addr_len = sizeof(client_addr);
	client_fd = accept(_server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_fd < 0)
	{
		close(_server_fd);
		log_exit("accept", __LINE__, __FILE__);
	}
	return client_fd;
}

void Server::childProcess(int client_fd)
{
	// TODO : buffer size調整、第3引数の調べる(MSG_DONTWAIT : ノンブロッキングモードなので使えそう)
	HTTPRequestParse request_parse(_request);
	ssize_t n = recv(client_fd, _buffer, sizeof(_buffer) - 1, 0);
	if (n < 0)
	{
		close(client_fd);
		log_exit("recv", __LINE__, __FILE__);
	}
	_buffer[n] = '\0';
	std::cout << "Received: " << _buffer << std::endl;
	request_parse.parse(_buffer);
	close(client_fd);
	std::cout << "Client connected" << std::endl;
	std::exit(EXIT_SUCCESS);
}

void Server::parentProcess(pid_t pid)
{
	int status;

	if (waitpid(pid, &status, 0) < 0)
	{
		close(_server_fd);
		log_exit("waitpid", __LINE__, __FILE__);
	}
}

void Server::mainLoop(void)
{
	int client_fd;
	for (;;)
	{
		// Accept
		client_fd =  acceptSocket();
		// Fork
		// TODO : signal handle forkはCGIの時だけにする　pollを使う
		pid_t pid = fork();
		if (pid < 0) {
			close(_server_fd);
			close(client_fd);
			log_exit("fork", __LINE__, __FILE__);
		}
		else if (pid == 0) {
			close(_server_fd);
			childProcess(client_fd);
		}
		else {
			close(client_fd);
			parentProcess(pid);
		}
	}
}

// TODO : split into smaller functions
void Server::start(void)
{
	std::cout << "================= Server::start =================" << std::endl;
	// Init server_addr
	initServerAddr();
	// Create socket
	createSocket();
	// TODO : search SIGCHLD
	// signal(SIGCHLD, SIG_IGN);
	// Bind
	bindSocket();
	// Listen
	listenSocket();
	// Main loop
	mainLoop();
	close(_server_fd);
}