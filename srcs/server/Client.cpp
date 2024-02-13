#include "Client.hpp"

Client::Client(int client_fd, int server_fd) :  _state(RECV_STATE), _request(), _request_parse(_request), _client_fd(client_fd), _server_fd(server_fd)
{
}


Client::~Client(void)
{
}

void Client::clear(void)
{
	_request.clear();
	_response.clear();
	_request_parse.clear();
	_state = RECV_STATE;
}

int Client::getClientFd(void) const
{
	return _client_fd;
}

int Client::getServerFd(void) const
{
	return _server_fd;
}

bool Client::getKeepAlive(void) const
{
	return _response.getKeepAlive();
}

ClientState Client::getState(void) const
{
	return _state;
}

HTTPRequestParseState Client::getHTTPRequestParseState(void) const
{
	return _request_parse.getHTTPRequestParseState();
}

const std::string &Client::getResponseMessage(void) const
{
	return _response.getResponseMessage();
}

HTTPResponse &Client::getResponse(void)
{
	return _response;
}

void Client::setClientFd(int client_fd)
{
	_client_fd = client_fd;
}

void Client::setServerFd(int server_fd)
{
	_server_fd = server_fd;
}

void Client::setState(ClientState state)
{
	_state = state;
}


int Client::recvProcess()
{

	char buffer[_buffer_size];
	std::cout << "client_fd: " << _client_fd << std::endl;
	ssize_t n = recv(_client_fd, buffer, sizeof(buffer) - 1, 0);
	if (n == 0) {
		return -1;
	} else if (n < 0) {
		log_print("recv", __LINE__, __FILE__, errno);
		return -1;
	}
	buffer[n] = '\0';
	#if DEBUG
		std::cout << "########## [ DEBUG ] request message ##########" << std::endl << buffer << std::endl
		<< "###############################################" << std::endl;
	#endif
	_request_parse.parse(buffer);
	if (_request_parse.getHTTPRequestParseState() == FINISH_STATE) {
		setState(SEND_STATE);
	}
	return 0;
}

void Client::responseProcess()
{
	std::string keep_alive = _request.getHeader("Connection");
	if (!keep_alive.empty() && keep_alive.compare("close") == 0)
		_response.setKeepAlive(false);
	_response.selectResponse(_request);
}

int Client::sendResponse(std::string &responseMessage)
{
	ssize_t send_n = send(_client_fd, responseMessage.c_str(), responseMessage.size(), 0);
	if (send_n < 0)
	{
		log_print("send", __LINE__, __FILE__, errno);
		return -1;
	}
	# if DEBUG
		std::cout << "########## send end ##########" << std::endl;
	#endif
	return 0;
}
