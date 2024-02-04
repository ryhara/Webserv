#include "HTTPResponse.hpp"

void HTTPResponse::makeGetResponseBody(HTTPRequest &request)
{
	std::string path = request.getUri();
	size_t found = path.find("?");
	if (found != std::string::npos) {
		path = path.substr(0, found);
	}
	// TODO : configの設定によって、pathを変更する ./www以外にも対応できるように
	if (isFileExist("./www/" + path, request.getStat()) == false) {
		std::ifstream ifs;
		// TODO : error_pageのパスをconfigから取得する
		makeFileBody(std::string("./www/error_page/404.html"));
	} else if (isFile(*request.getStat())) {
		makeFileBody("./www" + path);
	} else if (isDirectory(*request.getStat())) {
		// TODO : directoryの場合,configで設定されたファイルを返す, 今は必ずindex.htmlを返す
		makeFileBody(std::string("./www" + path + "/index.html"));
	} else {
		makeFileBody("./www/error_page/404.html");
	}
}