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
		ifs.open("./www/error_page/404.html");
		makeFileBody(ifs);
	} else if (isFile(*request.getStat())) {
		std::ifstream ifs;
		ifs.open("./www" + path);
		makeFileBody(ifs);
	} else if (isDirectory(*request.getStat())) {
		// TODO : directoryの場合,configで設定されたファイルを返す, 今は必ずindex.htmlを返す
		std::ifstream ifs;
		ifs.open("./www" + path + "/index.html");
		makeFileBody(ifs);
	} else {
		std::ifstream ifs;
		ifs.open("./www/error_page/404.html");
		makeFileBody(ifs);
	}
}