#include "HTTPResponse.hpp"

void HTTPResponse::makeGetResponseBody(HTTPRequest &request)
{
	std::string path = request.getUri();
	size_t found = path.find("?");
	if (found != std::string::npos) {
		path = path.substr(0, found);
	}
	// TODO : configの設定によって、pathを変更する ./www以外にも対応できるように
	if (isFileExist("./www" + path, request.getStat()) == false) {
		_statusCode = STATUS_404;
		makeFileBody("./www/error/404.html");
		return ;
	} else if (isFile(*request.getStat())) {
		makeFileBody("./www" + path);
	} else if (isDirectory(*request.getStat())) {
		// TODO : directoryの場合,configで設定されたファイルを返す, 今は必ずindex.htmlを返す
		makeFileBody(std::string("./www" + path + "/index.html"));
	} else {
		_statusCode = STATUS_404;
		makeFileBody("./www/error/404.html");
	}
}