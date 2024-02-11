#include "Client.hpp"


Client::Client(int client_fd, int server_fd) : _client_fd(client_fd), _server_fd(server_fd)
{
}

Client::~Client(void)
{
}

int Client::getClientFd(void) const
{
	return _client_fd;
}

int Client::getServerFd(void) const
{
	return _server_fd;
}

void Client::setClientFd(int client_fd)
{
	_client_fd = client_fd;
}

void Client::setServerFd(int server_fd)
{
	_server_fd = server_fd;
}


int Client::recvProcess(HTTPRequest &request)
{
	char buffer[_buffer_size];
	HTTPRequestParse request_parse(request);
	ssize_t n = recv(_client_fd, buffer, sizeof(buffer) - 1, 0);
	if (n < 0)
	{
		log_print("recv", __LINE__, __FILE__, errno);
		return -1;
	}
	buffer[n] = '\0';
	#if DEBUG
		std::cout << "########## [ DEBUG ] request message ##########" << std::endl << buffer << std::endl
		<< "###############################################" << std::endl;
	#endif
	request_parse.parse(buffer);
	return 0;
}

void Client::responseProcess(HTTPRequest &request, HTTPResponse &response)
{
	std::string keep_alive = request.getHeader("Connection");
	if (keep_alive == "close")
		response.setKeepAlive(false);
	response.selectResponse(request);
}

int Client::sendResponse(std::string &responseMessage)
{
	ssize_t send_n = send(_client_fd, responseMessage.c_str(), responseMessage.size(), 0);
	if (send_n < 0)
	{
		log_print("send", __LINE__, __FILE__, errno);
		return -1;
	}
	return 0;
}

int Client::clientProcess()
{
	HTTPRequest request;
	HTTPResponse response;
	HTTPRequestParse request_parse(request);
	std::string responseMessage;

	try {
		if (recvProcess(request) < 0)
			return -1;
		responseProcess(request, response);
	} catch (ServerException &e) {
		response.setStatusCode(e.getStatusCode());
		response.makeResponseMessage();
	}
	responseMessage = response.getResponseMessage();
	#if DEBUG
		std::cout << "########## [ DEBUG ] response message ##########" << std::endl << responseMessage << std::endl
			<< "################################################" << std::endl;
	#endif
	if (sendResponse(responseMessage) < 0)
		return -1;
	return 0;
	# if DEBUG
		std::cout << "==================================================" << std::endl;
	# endif
}
