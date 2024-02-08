#include "HTTPResponse.hpp"

void HTTPResponse::selectResponse(HTTPRequest &request)
{
	enum response_mode mode = request.getMode();
	switch (mode) {
	case NORMAL:
		handleNormalRequest(request);
		break;
	case CGI:
		handleCGIRequest(request);
		break;
	case AUTOINDEX:
		handleAutoIndexRequest(request);
		break;
	case REDIRECT:
		handleRedirectRequest(request);
		break;
	default:
		// TODO : error	message
		break;
	}
}

void HTTPResponse::handleNormalRequest(HTTPRequest &request)
{
	std::string method = request.getMethod();
	if (method == "GET") {
		makeGetResponseBody(request);
		setStatusLine();
		if (keepAlive)
			setHeader("Connection", "keep-alive");
		else
			setHeader("Connection", "close");
		setHeader("Date", getDateTimestamp());
		setHeader("Server", SERVER_NAME);
		setContentLength(_body.size());
		setHeader("Content-Length", ft_stoi(_contentLength));
		_responseMessage += _statusLine;
		for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++) {
			_responseMessage += it->first + ": " + it->second + CRLF;
		}
		_responseMessage += CRLF;
		_responseMessage += _body;
	} else if (method == "POST") {
		makePostResponseBody(request);
	} else if (method == "DELETE") {
		makeDeleteResponseBody(request);
	}
	else {
		// TODO : error message
	}
}

void HTTPResponse::handleCGIRequest(HTTPRequest &request)
{
	// TODO : CGIの処理
	std::cout << request.getUri() << std::endl;
}

void HTTPResponse::handleAutoIndexRequest(HTTPRequest &request)
{
	// TODO : autoindexの処理
	std::cout << request.getUri() << std::endl;
}

void HTTPResponse::handleRedirectRequest(HTTPRequest &request)
{
	// TODO : redirectの処理
	std::cout << request.getUri() << std::endl;
}

void HTTPResponse::handleErrorResponse(HTTPRequest &request)
{
	// TODO : errorの処理
	std::cout << request.getUri() << std::endl;
}