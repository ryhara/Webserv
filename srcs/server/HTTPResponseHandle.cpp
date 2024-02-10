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
		throw InternalServerError();
		break;
	}
}

void HTTPResponse::handleNormalRequest(HTTPRequest &request)
{
	// TODO : GETは必須、POSTとDELETEは任意で, configで許可されていない場合405 Method Not Allowed
	std::string method = request.getMethod();
	if (method == "GET") {
		makeGetResponseBody(request);
		makeResponseMessage();
	} else if (method == "POST") {
		makePostResponseBody(request);
		makeResponseMessage();
	} else if (method == "DELETE") {
		makeDeleteResponseBody(request);
		makeResponseMessage();
	}
	else {
		throw NotImplementedError();
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