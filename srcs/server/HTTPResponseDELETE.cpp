#include "HTTPResponse.hpp"


// TODO : 相対パスの挙動調査(curlでリクエストの出力すると良いかも)、相対パスはcurlやブラウザでかき消されてるかもしれない
void HTTPResponse::makeDeleteResponseBody(HTTPRequest &request)
{
	std::string path = request.getUri();
	// TODO : configの設定によって、pathを変更する ./www以外にも対応できるように
	path = "./www" + path;
	if (isFileExist(path, request.getStat()) == false) {
		_statusCode = STATUS_404;
		return ;
	} else if (isFile(*request.getStat())) {
		if ((std::remove(path.c_str()) != 0)) {
			_statusCode = STATUS_403;
			return ;
		} else {
			_statusCode = STATUS_204;
			return ;
		}
	} else if (isDirectory(*request.getStat())) {
		_statusCode = STATUS_403;
		return ;
	} else {
		_statusCode = STATUS_500;
		return ;
	}
}
