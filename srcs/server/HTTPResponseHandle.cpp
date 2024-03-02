#include "HTTPResponse.hpp"


bool HTTPResponse::isCGI(std::vector<std::string> &cgi_extension, std::string extension)
{
	for (std::vector<std::string>::iterator it = cgi_extension.begin(); it != cgi_extension.end(); it++) {
		if (extension.compare(*it) == 0) {
			return (true);
		}
	}
	return (false);
}

void HTTPResponse::selectResponse(HTTPRequest &request)
{
	Location location = request.getServerConfig().getLocation(request.getLocation());
	std::vector<std::string> cgi_extension = location.getCgiExtension();
	std::string uri = request.getUri();
	std::string extension = "";
	if (uri.find_last_of(".") != std::string::npos) {
		extension = uri.substr(uri.find_last_of("."));
		extension = extension.substr(0, extension.find("?"));
	}
	if (location.getRedirPath().empty() == false) {
		request.setMode(REDIRECT);
	} else if (extension.empty() == false && isCGI(cgi_extension, extension)) {
		request.setMode(CGI);
	} else {
		request.setMode(NORMAL);
	}
	enum response_mode mode = request.getMode();
	switch (mode) {
		case NORMAL:
			if (request.getServerConfig().getLocation(request.getLocation()).getAutoindex() == true) {
				#ifdef DEBUG
					std::cout << "########## [ DEBUG ] AUTOINDEX ##########" << std::endl;
				#endif
				handleAutoIndexRequest(request);
			} else {
				#ifdef DEBUG
					std::cout << "########## [ DEBUG ] NORMAL ##########" << std::endl;
				#endif
				handleNormalRequest(request);
			}
			break;
		case CGI:
		#ifdef DEBUG
			std::cout << "########## [ DEBUG ] CGI ##########" << std::endl;
		#endif
			handleCGIRequest(request);
			break;
		case REDIRECT:
		#ifdef DEBUG
			std::cout << "########## [ DEBUG ] REDIRECT ##########" << std::endl;
		#endif
			handleRedirectRequest(request);
			break;
		default:
			throw ServerException(STATUS_500, "Internal Server Error");
			break;
	}
}

void HTTPResponse::handleNormalRequest(HTTPRequest &request)
{
	std::string method = request.getMethod();
	Location location = request.getServerConfig().getLocation(request.getLocation());
	if (method.compare("GET") == 0) {
		if (location.getGetMethod() == false)
			throw ServerException(STATUS_405, "Method Not Allowed");
		makeGetResponseBody(request);
		makeResponseMessage();
	} else if (method.compare("POST") == 0) {
		if (location.getPostMethod() == false)
			throw ServerException(STATUS_405, "Method Not Allowed");
		makePostResponseBody(request);
		makeResponseMessage();
	} else if (method.compare("DELETE") == 0) {
		if (location.getDeleteMethod() == false)
			throw ServerException(STATUS_405, "Method Not Allowed");
		makeDeleteResponseBody(request);
		makeResponseMessage();
	}
	else {
		throw ServerException(STATUS_501, "Not Implemented");
	}
}

void HTTPResponse::handleCGIRequest(HTTPRequest &request)
{
	std::cout << request.getUri() << std::endl;
}

void HTTPResponse::handleAutoIndexRequest(HTTPRequest &request)
{
	std::stringstream ss;
	std::string uri = request.getUri();
	std::vector<std::string> file_list;
	Location location = request.getServerConfig().getLocation(request.getLocation());
	std::string new_uri = uri.substr(location.getLocation().size(), uri.size());
	std::string path = location.getAlias() + new_uri;
	if (path[path.size() - 1] != '/') {
		handleNormalRequest(request);
		return ;
	} else if (isFileExist(path + location.getIndex(), request.getStat())) {
		handleNormalRequest(request);
		return ;
	}
	readDirectory(file_list, path);
	makeAutoindexBody(file_list, location.getAlias(), location.getLocation(), ss);
	makeResponseMessage();
}

void HTTPResponse::handleRedirectRequest(HTTPRequest &request)
{
	std::string uri = request.getUri();
	std::string path = "";
	Location location = request.getServerConfig().getLocation(request.getLocation());
	std::string redirect_path = request.getServerConfig().getLocation(request.getLocation()).getRedirPath();
	// リダイレクトの後ろのパスも設定する場合
	// size_t found = uri.find(location.getLocation());
	// if (found != std::string::npos) {
	// 	path = uri.substr(found + location.getLocation().size(), uri.size());
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

void HTTPResponse::makeAutoindexBody(std::vector<std::string> file_list, std::string alias, std::string location, std::stringstream &ss)
{
	ss << "<html><head><title>Index of " + location + "</title></head><body><h1>Index of " + location + "</h1><hr><pre style=\"font-family: Courier, Consolas, 'Courier New', monospace;\">";
	for (std::vector<std::string>::iterator it = file_list.begin(); it != file_list.end(); it++) {
		struct stat s_stat;
		std::string file_name = *it;
		std::string file_path = alias + file_name;
		if (isFileExist(file_path, &s_stat)) {
			char time_buf[DATE_BUF_SIZE] = {0};
			std::strftime(time_buf, sizeof(time_buf), "%d-%m-%Y %H:%M", std::localtime(&s_stat.st_mtime));
			std::string time(time_buf);
			size_t file_size = s_stat.st_size;
			if (isDirectory(s_stat)) {
				if (file_name.compare("..") == 0) {
					ss << "<a href=\"../\">" << ".." << "/</a>" << CRLF;
					continue;
				}
				ss <<  "<a href=\"" << location << file_name << "/\">" << file_name << "/</a>" << std::setw(50 - file_name.size() - 1) << time << std::setw(20) << "-" << CRLF;
			} else if (isFile(s_stat)) {
				ss << "<a href=\"" << location << file_name << "\">" << file_name << "</a>" << std::setw(50 - file_name.size()) << time << std::setw(20) << ft_to_string(file_size) << CRLF;
			}
		} else {
			throw ServerException(STATUS_404, "Not Found");
		}
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
		throw ServerException(STATUS_404, "Not Found");
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
