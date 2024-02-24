#include "HTTPResponse.hpp"

void HTTPResponse::makeGetResponseBody(HTTPRequest &request)
{
	std::string uri = request.getUri();
	size_t found = uri.find("?");
	if (found != std::string::npos) {
		uri = uri.substr(0, found);
	}
	Location location = request.getServerConfig().getLocation(request.getLocation());
	std::string alias = location.getAlias();
	std::string new_uri = uri.substr(location.getLocation().size(), uri.size());
	std::string path = alias + new_uri;
	if (isFileExist(path, request.getStat()) == false) {
		_statusCode = STATUS_404;
		makeFileBody(request.getServerConfig().getErrorPage());
		return ;
	} else if (isFile(*request.getStat())) {
		makeFileBody(path);
	} else if (isDirectory(*request.getStat()) && path[path.size() - 1] == '/') {
		std::cout << "directory" << std::endl;
		makeFileBody(std::string(path + location.getIndex()));
	} else {
		_statusCode = STATUS_404;
		makeFileBody(request.getServerConfig().getErrorPage());
	}
}