#include "HTTPResponse.hpp"

void HTTPResponse::selectResponse(HTTPRequest &request)
{
	enum response_mode mode = request.getMode();
	switch (mode) {
	case NORMAL:
	#ifdef DEBUG
		std::cout << "########## [ DEBUG ] NORMAL ##########" << std::endl;
	#endif
		handleNormalRequest(request);
		break;
	case CGI:
	#ifdef DEBUG
		std::cout << "########## [ DEBUG ] CGI ##########" << std::endl;
	#endif
		handleCGIRequest(request);
		break;
	case AUTOINDEX:
	#ifdef DEBUG
		std::cout << "########## [ DEBUG ] AUTOINDEX ##########" << std::endl;
	#endif
		handleAutoIndexRequest(request);
		break;
	case REDIRECT:
	#ifdef DEBUG
		std::cout << "########## [ DEBUG ] REDIRECT ##########" << std::endl;
	#endif
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

// TODO : curlは成功、ブラウザはEAGAINでexitする
void HTTPResponse::handleRedirectRequest(HTTPRequest &request)
{
	std::string uri = request.getUri();
	std::string location = request.getLocation();
	std::string path = "";
	// TODO : configで設定されたredirectのpathを取得する
	std::string redirect_path = "http://google.com";
	// TODO : リダイレクトの後ろのパスも設定する場合
	// size_t found = uri.find(location);
	// if (found != std::string::npos) {
	// 	path = uri.substr(found + location.size(), uri.size());
	// }
	// path = redirect_path + "/" + path;
	path = redirect_path;
	setHeader("Location", path);
	setStatusCode(STATUS_301);
	makeResponseMessage();
}

void HTTPResponse::handleErrorResponse(HTTPRequest &request)
{
	// TODO : errorの処理
	std::cout << request.getUri() << std::endl;
}