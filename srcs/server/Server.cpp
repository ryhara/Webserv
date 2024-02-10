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
	_server_fds.clear();
	if (_res)
		freeaddrinfo(_res);
}

Config &Server::getConfig(void) const
{
	return const_cast<Config &>(_config);
}

void Server::setPortAndServerFd(std::string &port, int server_fd)
{
	_server_fds[port] = server_fd;
}

void Server::closeServerFds(void)
{
	for (std::map<std::string, int>::iterator it = _server_fds.begin(); it != _server_fds.end(); ++it)
	{
		close(it->second);
	}
}

void Server::initServerAddr(std::string &port)
{
	char *port_str = const_cast<char *>(port.c_str());
	ft_memset(&_hints, 0, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, port_str, &_hints, &_res) != 0)
		log_exit("getaddrinfo", __LINE__, __FILE__, errno);
}

void Server::createSocket(std::string &port)
{
	int option_value = 1; // setsockopt
	int server_fd;

	server_fd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	if (server_fd < 0) {
		closeServerFds();
		log_exit("socket", __LINE__, __FILE__, errno);
	}
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&option_value, sizeof(option_value)) < 0)
	{
		close(server_fd);
		closeServerFds();
		log_exit("setsockopt", __LINE__, __FILE__, errno);
	}
	setNonBlockingFd(server_fd);
	setPortAndServerFd(port, server_fd);
}

void Server::bindSocket(int &server_fd)
{
	if (bind(server_fd, _res->ai_addr, _res->ai_addrlen) < 0)
	{
		closeServerFds();
		freeaddrinfo(_res);
		_res = NULL;
		log_exit("bind", __LINE__, __FILE__, errno);
	}
	freeaddrinfo(_res);
	_res = NULL;
}

void Server::listenSocket(int &server_fd)
{
	if (listen(server_fd, QUEUE_LENGTH) < 0)
	{
		closeServerFds();
		log_exit("listen", __LINE__, __FILE__, errno);
	}
}

int Server::acceptSocket(int &server_fd)
{
	int 				client_fd;
	socklen_t 			client_addr_len;
	struct sockaddr_in	client_addr;

	client_addr_len = sizeof(client_addr);
	client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
	if (client_fd < 0) {
		return -1;
	}
	return client_fd;
}

// TODO : Clientクラスへ以降、頃合いを見て削除
void Server::clientProcess(int client_fd)
{
	HTTPRequest request;
	HTTPResponse response;
	HTTPRequestParse request_parse(request);
	std::string responseMessage;
	ssize_t n = recv(client_fd, _buffer, sizeof(_buffer) - 1, 0);
	if (n < 0)
	{
		close(client_fd);
		closeServerFds();
		log_exit("recv", __LINE__, __FILE__, errno);
	}
	_buffer[n] = '\0';
	#if DEBUG
		std::cout << "#### [ DEBUG ] request message ####" << std::endl << _buffer << std::endl << "##########" << std::endl;
	#endif
	try {
		request_parse.parse(_buffer);
		response.selectResponse(request);
		responseMessage = response.getResponseMessage();
	} catch (HTTPRequestParseError &e) {
		std::cerr << e.what() << std::endl;
		response.setStatusCode(STATUS_400);
		response.setStatusLine();
		responseMessage = response.getStatusLine();
	} catch (InternalServerError &e) {
		std::cerr << e.what() << std::endl;
		response.setStatusCode(STATUS_500);
		response.setStatusLine();
		responseMessage = response.getStatusLine();
	} catch (HTTPRequestPayloadTooLargeError &e) {
		std::cerr << e.what() << std::endl;
		response.setStatusCode(STATUS_413);
		response.setStatusLine();
		responseMessage = response.getStatusLine();
	} catch (std::exception &e) {
		std::cerr << e.what() << std::endl;
		response.setStatusCode(STATUS_500);
		response.setStatusLine();
		responseMessage = response.getStatusLine();
	}
	#if DEBUG
		std::cout << "##### [ DEBUG ] response message ####" << std::endl << responseMessage << std::endl << "##########" << std::endl;
	#endif
	ssize_t send_n = send(client_fd, responseMessage.c_str(), responseMessage.size(), 0);
	if (send_n < 0)
	{
		close(client_fd);
		closeServerFds();
		log_exit("send", __LINE__, __FILE__, errno);
	}
	# if DEBUG
		std::cout << "==================================================" << std::endl;
	# endif
}

void Server::pollInit(void)
{

	int index = 0;
	for (std::map<std::string, int>::iterator it = _server_fds.begin(); it != _server_fds.end(); ++it)
	{
		this->fds_[index].fd = it->second;
		this->fds_[index].events = POLLIN;
		this->fds_[index].revents = 0;
		index++;
	}
	for (int i = index; i < MAX_CLIENTS + 1; ++i)
	{
		this->fds_[i].fd = -1;
		this->fds_[i].events = POLLIN;
		this->fds_[i].revents = 0;
	}
}

void Server::mainLoop(void)
{
	int client_fd;

	pollInit();
	for (;;) {
		int	result = poll(fds_, MAX_CLIENTS, -1);
		if (result < 0) {
			log_exit("poll", __LINE__, __FILE__, errno);
		}
		else if (result == 0) {
			// timeout
			std::cout << "-- poll timeout --" << std::endl;
			continue;
		}
		for (int i = 0; i < MAX_CLIENTS; ++i) {
			if (fds_[i].fd != -1 && (fds_[i].revents & (POLLIN | POLLERR))) {
				client_fd = acceptSocket(fds_[i].fd);
				if (client_fd < 0) {
					if (errno == EWOULDBLOCK || errno == EAGAIN) {
						continue;
					} else {
						closeServerFds();
						log_exit("accept", __LINE__, __FILE__, errno);
					}
				}
				setNonBlockingFd(client_fd);
				Client client(client_fd, fds_[i].fd);
				if (client.clientProcess() < 0) {
					close(client_fd);
					closeServerFds();
					log_exit("clientProcess", __LINE__, __FILE__, errno);
				}
				close(client_fd);
			}
		}
	}
}

void Server::start(void)
{
	std::cout << "================= Server start =================" << std::endl;
	std::vector<std::string> list;
	list.push_back("4242");
	list.push_back("4243");
	// TODO : MAX_CLIENTSを超えた分はエラーを返す. CONFIG段階でエラーにしとく
	if (list.size() > MAX_CLIENTS)
	{
		log_exit("server start", __LINE__, __FILE__, errno);
	}
	// TODO : configのサーバーの数だけループする。
	for (int i = 0; i < 2; ++i) {
		std::string current_server_port = list[i];
		initServerAddr(list[i]);
		createSocket(current_server_port);
		int current_server_fd = _server_fds[list[i]];
		bindSocket(current_server_fd);
		listenSocket(current_server_fd);
	}
	mainLoop();
	closeServerFds();
}
