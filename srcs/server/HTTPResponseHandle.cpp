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
			// TODO : on/offの設定を確認する, offの場合はhandleNormalRequestを呼ぶ
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
	// Method Not Allowedの場合は[Allow: GET, POST, DELETE]のようにAllowヘッダをつける
	std::string method = request.getMethod();
	if (method.compare("GET") == 0) {
		makeGetResponseBody(request);
		makeResponseMessage();
	} else if (method.compare("POST") == 0) {
		makePostResponseBody(request);
		makeResponseMessage();
	} else if (method.compare("DELETE") == 0) {
		makeDeleteResponseBody(request);
		makeResponseMessage();
	}
	else {
		throw NotImplementedError();
	}
}

void HTTPResponse::handleCGIRequest(HTTPRequest &request)
{
	std::cout << request.getUri() << std::endl;
}

void HTTPResponse::makeAutoindexBody(std::vector<std::string> file_list, std::string uri, std::string path, std::stringstream &ss)
{
	ss << "<html><head><title>Index of " + path + "</title></head><style> .file-list {font-family: monospace;}</style><body><h1>Index of " + path + "</h1><hr><pre class=\"file-list\">";
	for (std::vector<std::string>::iterator it = file_list.begin(); it != file_list.end(); it++) {
		struct stat s_stat;
		std::string file_path = path + *it;
		if (isFileExist(file_path, &s_stat)) {
			char time_buf[DATE_BUF_SIZE] = {0};
			std::strftime(time_buf, sizeof(time_buf), "%d-%m-%Y %H:%M", std::localtime(&s_stat.st_mtime));
			std::string time(time_buf);
			size_t file_size = s_stat.st_size;
			std::string file_name = *it;
			if (isDirectory(s_stat)) {
				if (file_name.compare("..") == 0) {
					ss << "<a href=\"../\">" << ".." << "/</a>" << CRLF;
					continue;
				}
				ss <<  "<a href=\"" << uri << file_name << "/\">" << file_name << "/</a>" << std::setw(50 - file_name.size() + 1) << time << std::setw(20) << "-" << CRLF;
			} else if (isFile(s_stat)) {
				ss << "<a href=\"" << uri << file_name << "\">" << file_name << "</a>" << std::setw(50 - file_name.size()) << time << std::setw(20) << ft_to_string(file_size) << CRLF;
			}
		} else
			throw NotFoundError();
	}
	ss << "</pre><hr></body></html>\r\n" ;
	std::string html = ss.str();
	setBody(html);
}

void HTTPResponse::readDirectory(std::vector<std::string> &file_list, const std::string &path)
{
	DIR *dir;
	struct dirent *dp;
	dir = opendir(path.c_str());
	if (dir == NULL) {
		throw NotFoundError();
	} else {
		while ((dp = readdir(dir)) != NULL) {
			std::string file_name = dp->d_name;
			if (file_name.compare(".") == 0) {
				continue;
			} else {
				file_list.push_back(file_name);
			}
		}
	}
	closedir(dir);
}

void HTTPResponse::handleAutoIndexRequest(HTTPRequest &request)
{
	std::stringstream ss;
	std::string uri = request.getUri();
	std::vector<std::string> file_list;
	// TODO : configで設定されたautoindexのpathを取得する
	// TODO : autoindexのデフォルトのindex.htmlがあればmakeGetResponseBodyと同様な処理
	std::string path = "./www" + uri;
	if (path[path.size() - 1] != '/') {
		handleNormalRequest(request);
		return ;
	}
	readDirectory(file_list, path);
	// TODO : pathはconfigで設定されたautoindexのpathを取得する
	makeAutoindexBody(file_list, uri, path, ss);
	makeResponseMessage();
}

void HTTPResponse::handleRedirectRequest(HTTPRequest &request)
{
	std::string uri = request.getUri();
	std::string location = request.getLocation();
	std::string path = "";
	// TODO : configで設定されたredirectのpathを取得する
	std::string redirect_path = "http://google.com";
	// リダイレクトの後ろのパスも設定する場合
	// size_t found = uri.find(location);
	// if (found != std::string::npos) {
	// 	path = uri.substr(found + location.size(), uri.size());
	// }
	// path = redirect_path + "/" + path;
	path = redirect_path;
	setHeader("Location", path);
	if (request.getMethod().compare("GET") == 0) {
		setStatusCode(STATUS_301);
	} else {
		setStatusCode(STATUS_308);
	}
	makeResponseMessage();
}
