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

Config &Server::getConfig(void) const
{
	return const_cast<Config &>(_config);
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
	if (listen(_server_fd, QUEUE_LENGTH) < 0)
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
	std::cout << "-- request -- " << std::endl;
	for (int i = 0; i < n; i++)
		std::cout << _buffer[i];
	std::cout << "--------------" << std::endl;
	request_parse.parse(_buffer);
	// TODO : responseを正しく実装する （一時的）
	std::string _response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello, World!</h1></body></html>\r\n";
	ssize_t send_n = send(client_fd, _response.c_str(), _response.size(), 0);
	if (send_n < 0)
	{
		close(client_fd);
		log_exit("send", __LINE__, __FILE__);
	}
	close(client_fd);
	std::cout << "================= one request end =================" << std::endl;
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
	int client_fd, kq, event_number;
	struct kevent change_event;
	struct kevent events[MAX_EVENTS];

	// TODO : kqueueを分割, childProcess, ParentProcessを修正
	kq = kqueue();
	if (kq < 0)
	{
		close(_server_fd);
		log_exit("kqueue", __LINE__, __FILE__);
	}

	EV_SET(&change_event, _server_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &change_event, 1, NULL, 0, NULL) < 0)
	{
		close(_server_fd);
		log_exit("kevent", __LINE__, __FILE__);
	}

	for (;;) {
		event_number = kevent(kq, NULL, 0, events, MAX_EVENTS, NULL);
		if (event_number < 0)
		{
			close(_server_fd);
			log_exit("kevent", __LINE__, __FILE__);
		}
		std::cout << "event_number : " << event_number << std::endl;
		for (int i = 0; i < event_number; i++) {
			if (events[i].flags & EV_ERROR) {
				close(events[i].ident);
				log_exit("EV_ERROR", __LINE__, __FILE__);
			} else if (events[i].filter == EVFILT_READ) {
				// TODO : 上下のif文の条件を調べる, 下のif文なくても動く
				// if (static_cast<int>(events[i].ident) == _server_fd) {
					std::cout << "events[" << i << "].ident : " << events[i].ident << std::endl;
					std::cout << "_server_fd : " << _server_fd << std::endl;
					client_fd = acceptSocket();
					EV_SET(&change_event, client_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
					if (kevent(kq, &change_event, 1, NULL, 0, NULL) < 0)
					{
						close(_server_fd);
						log_exit("kevent", __LINE__, __FILE__);
					}
					childProcess(client_fd);
				// }
			}
		}
	}
	close(kq);
}

void Server::start(void)
{
	std::cout << "================= Server::start =================" << std::endl;
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

/* example */
// TODO : fcntlを使ったノンブロッキングモードの実装または pollを使った実装
// if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
//     perror("fcntl");
//     close(fd);
//     exit(EXIT_FAILURE);
// }