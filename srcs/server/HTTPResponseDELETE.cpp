#include "HTTPResponse.hpp"


// TODO : 相対パスはcurlやブラウザでかき消されてるかもしれない
void HTTPResponse::makeDeleteResponseBody(HTTPRequest &request)
{
	std::string path = request.getUri();
	// TODO : configの設定によって、pathを変更する ./www以外にも対応できるように
	path = "./www" + path;
	if (isFileExist(path, request.getStat()) == false) {
		_statusCode = STATUS_404;
		_responseMessage = "HTTP/1.1 404 Not Found\r\n";
		return ;
	} else if (isFile(*request.getStat())) {
		if ((std::remove(path.c_str()) != 0)) {
			_statusCode = STATUS_403;
			_responseMessage = "HTTP/1.1 403 Forbidden\r\n";
			return ;
		} else {
			_statusCode = STATUS_204;
			_responseMessage = "HTTP/1.1 204 No Content\r\n";
			return ;
		}
	} else if (isDirectory(*request.getStat())) {
		_statusCode = STATUS_403;
		_responseMessage = "HTTP/1.1 403 Forbidden\r\n";
		return ;
	} else {
		_statusCode = STATUS_500;
		_responseMessage = "HTTP/1.1 500 Internal Server Error\r\n";
		return ;
	}
}
