#include "Server.hpp"

Server::Server(void)
{
	ft_memset(&_hints, 0, sizeof(addrinfo));
	_res = NULL;
	ft_memset(_buffer, 0, sizeof(char) * BUFFER_SIZE);
}

Server::~Server(void)
{
	_server_fds.clear();
	FD_ZERO(&_readfds);
	if (_res)
		freeaddrinfo(_res);
}

void	Server::setServers(std::map<int, std::vector<ServerConfig> > &servers)
{
	_servers = servers;
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
	for (std::map<std::string, int>::iterator it = _server_fds.begin(); it != _server_fds.end(); ++it) {
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
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&option_value, sizeof(option_value)) < 0) {
		close(server_fd);
		closeServerFds();
		log_exit("setsockopt", __LINE__, __FILE__, errno);
	}
	setNonBlockingFd(server_fd);
	setPortAndServerFd(port, server_fd);
}

void Server::bindSocket(int &server_fd)
{
	if (bind(server_fd, _res->ai_addr, _res->ai_addrlen) < 0) {
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
	if (listen(server_fd, QUEUE_LENGTH) < 0) {
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

void Server::initFds(void)
{
	FD_ZERO(&_readfds);
	FD_ZERO(&_writefds);
	for (std::map<std::string, int>::iterator it = _server_fds.begin(); it != _server_fds.end(); ++it) {
		FD_SET(it->second, &_readfds);
	}
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		if (it->second->getState() == RECV_STATE)
			FD_SET(it->first, &_readfds);
		else if (it->second->getState() == SEND_STATE)
			FD_SET(it->first, &_writefds);
		// TODO : CGI
		// else if (it->second->getRequest().getMode() == CGI_MODE)

	}

}

void Server::serverEvent(void)
{
	for (std::map<std::string, int>::iterator it = _server_fds.begin(); it != _server_fds.end(); ++it) {
		if (FD_ISSET(it->second, &_readfds)) {
			FD_CLR(it->second, &_readfds);
			_server_fd = it->second;
			int client_fd = acceptSocket(it->second);
			if (client_fd < 0) {
				deleteAllClients();
				closeServerFds();
				log_exit("accept", __LINE__, __FILE__, errno);
			}
			setNonBlockingFd(client_fd);
			Client *client = new Client(client_fd, it->second);
			_clients.insert(std::make_pair(client_fd, client));
			break;
		}
	}
}

void Server::executeRecvProcess(std::map<int, Client*>::iterator &it)
{
	Client *client = it->second;
	if (client->recvProcess() < 0)
		deleteClient(it);
}

void Server::executeSendProcess(std::map<int, Client*>::iterator &it)
{
	Client *client = it->second;
	std::string port = client->getRequest().getPort();
	std::string host = client->getRequest().getHost();
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);
	if (getsockname(it->first, (struct sockaddr *)&addr, &len) == -1) {
		deleteAllClients();
		closeServerFds();
		log_exit("getsockname", __LINE__, __FILE__, errno);
	}
	if (ft_stoi(port) != ntohs(addr.sin_port))
		throw BadRequestError();
	int config_index = 0;
	for (int i = 0;i < static_cast<int>(_servers[ft_stoi(port)].size()); i++) {
		if (_servers[ft_stoi(port)][i].getServerName() == host) {
			config_index = i;
			break;
		}
	}
	client->getRequest().setServerConfig(_servers[ft_stoi(port)][config_index]);
	if (_servers[ft_stoi(port)][config_index].getMaxBodySize() < client->getRequest().getBody().size())
		throw HTTPRequestPayloadTooLargeError();
	client->responseProcess();
	std::string responseMessage = client->getResponseMessage();
	if (client->sendResponse(responseMessage) < 0)
		deleteClient(it);
}

void Server::deleteClient(std::map<int, Client*>::iterator &it)
{
	close(it->first);
	delete it->second;
	it->second = NULL;
	_clients.erase(it);
}

void Server::deleteAllClients(void)
{
	for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		close(it->first);
		delete it->second;
		it->second = NULL;
	}
	_clients.clear();
}

void Server::mainLoop(void)
{
	std::string responseMessage = "";
	struct timeval timeout;
	timeout.tv_sec = 30;
	timeout.tv_usec = 0;
	while (1) {
		initFds();
		// TODO : max_fdを設定する
		int result = select(FD_SETSIZE, &_readfds, &_writefds, NULL, &timeout);
		if (result < 0) {
			closeServerFds();
			log_exit("select", __LINE__, __FILE__, errno);
		} else if (result == 0) {
			// timeout
			#ifdef DEBUG
				std::cout << "[ DEBUG ] timeout" << std::endl;
			#endif
			deleteAllClients();
		} else {
			serverEvent();
			for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
			{
				try {
					if (FD_ISSET(it->first, &_readfds)) {
						FD_CLR(it->first, &_readfds);
						if (it->second->getState() == RECV_STATE) {
							executeRecvProcess(it);
							break;
						}
					} else if (FD_ISSET(it->first, &_writefds)) {
						FD_CLR(it->first, &_writefds);
						if (it->second->getState() == SEND_STATE) {
							executeSendProcess(it);
						}
						if (it->second->getKeepAlive() == false)
							deleteClient(it);
						else
							it->second->clear();
						break;
					}
				} catch (ServerException &e) {
					Client *client = it->second;
					client->getResponse().setStatusCode(e.getStatusCode());
					client->getResponse().makeResponseMessage();
					responseMessage = client->getResponseMessage();
					client->sendResponse(responseMessage);
					client->setState(RECV_STATE);
					if (client->getKeepAlive() == false)
						deleteClient(it);
					else
						client->clear();
					break;
				}
			}
		}
	}
}

void Server::start(void)
{
	#ifdef DEBUG
		std::cout << "================= Server start =================" << std::endl;
	#endif
	if (_servers.size() > FD_SETSIZE / 2)
	{
		log_exit("server start", __LINE__, __FILE__, errno);
	}
	for (std::map<int, std::vector<ServerConfig> >::iterator it = _servers.begin(); it != _servers.end(); ++it)
	{
		std::vector<ServerConfig> server_configs = it->second;
		std::string current_port = ft_to_string(static_cast<int>(it->first));
		initServerAddr(current_port);
		createSocket(current_port);
		int current_server_fd = _server_fds[current_port];
		bindSocket(current_server_fd);
		listenSocket(current_server_fd);
	}
	mainLoop();
	closeServerFds();
}