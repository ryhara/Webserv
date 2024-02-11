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
	std::string uploadPath = "./uploads/";
	std::string header = "post_";
	std::ofstream ofs;

	// TODO : configから保存先を取得する
	ofs.open(uploadPath + header + timestamp);
	if (!ofs.is_open())
	{
		_statusCode = STATUS_500;
		return;
	}
	ofs << body;
	ofs.close();
	_statusCode = STATUS_201;
}
