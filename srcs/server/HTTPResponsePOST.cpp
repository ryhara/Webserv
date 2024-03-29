#include "HTTPResponse.hpp"


std::string HTTPResponse::getTimeStampForPost(void) const
{
	std::string ret;
	time_t now = std::time(NULL);
	now += (9 * 60 * 60); // JST
	struct tm *gmt = std::gmtime(&now);
	char buf[DATE_BUF_SIZE] = {0};

	std::strftime(buf, sizeof(buf), "%y%m%d_%H%M%S", gmt);
	ret = buf;
	return ret;
}

void HTTPResponse::makePostResponseBody(HTTPRequest &request)
{
	std::string body = request.getBody();
	std::string timestamp = getTimeStampForPost();
	std::string uploadPath = request.getServerConfig().getLocation(request.getLocation()).getUploadPath();
	std::string header = "post_";
	std::ofstream ofs;

	ofs.open((uploadPath + header + timestamp).c_str());
	if (!ofs.is_open())
	{
		_statusCode = STATUS_500;
		return;
	}
	ofs << body;
	ofs.close();
	_statusCode = STATUS_201;
}
