#include "HTTPResponse.hpp"


// TODO : 相対パスはcurlやブラウザでかき消されてるかもしれない
void HTTPResponse::makeDeleteResponseBody(HTTPRequest &request)
{
	std::string path = request.getUri();
	// TODO : configの設定によって、pathを変更する ./www以外にも対応できるように
	path = "./www" + path;
	if (isFileExist(path, request.getStat()) == false) {
		// TODO : file not found
		_body = "<html><body><h1>DELETE " + path + " not found !!</h1></body></html>\r\n";
		return ;
	} else if (isFile(*request.getStat())) {
		if ((std::remove(path.c_str()) != 0)) {
			// TODO : error
			_body = "<html><body><h1>DELETE " + path + " failed !!</h1></body></html>\r\n";
			return ;
		} else {
			// TODO : OK
			_body = "<html><body><h1>DELETE " + path + " successfully !!</h1></body></html>\r\n";
			return ;
		}
	} else if (isDirectory(*request.getStat())) {
		// TODO : forbidden
		_body = "<html><body><h1>DELETE " + path + " is directory !! </h1></body></html>\r\n";
		return ;
	} else {
		// TODO : error
		return ;
	}
}
