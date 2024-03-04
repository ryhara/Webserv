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
		std::string index = location.getIndex();
		if (isFileExist(path + index, request.getStat()) && isFile(*request.getStat())) {
			makeFileBody(path + index);
		} else {
			_statusCode = STATUS_404;
			makeFileBody(request.getServerConfig().getErrorPage());
		}
	} else {
		_statusCode = STATUS_404;
		makeFileBody(request.getServerConfig().getErrorPage());
	}
}
