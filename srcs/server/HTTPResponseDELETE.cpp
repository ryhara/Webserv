#include "HTTPResponse.hpp"

void HTTPResponse::makeDeleteResponseBody(HTTPRequest &request)
{
	std::string uri = request.getUri();
	Location location = request.getServerConfig().getLocation(request.getLocation());
	std::string alias = location.getAlias();
	std::string new_uri = uri.substr(location.getLocation().size(), uri.size());
	std::string path = alias + new_uri;
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
