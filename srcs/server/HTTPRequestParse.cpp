#include "HTTPRequestParse.hpp"

HTTPRequestParse::HTTPRequestParse(HTTPRequest &request) : _request(request)
{
}

HTTPRequestParse::~HTTPRequestParse(void)
{
}

HTTPRequest &HTTPRequestParse::getRequest(void) const
{
	return (this->_request);
}

int HTTPRequestParse::getlineWithCRLF(std::stringstream &ss, std::string &line)
{
	if (!std::getline(ss, line))
		return (0);
	if (line == "\r")
		return (0);
	return (1);
}

void HTTPRequestParse::parse(char *buffer)
{
	std::string line;
	std::stringstream ss(buffer);

	// TODO : 1行ずつ読み込む
	if (!getlineWithCRLF(ss, line))
		log_exit("getline", __LINE__, __FILE__);
	readRequestLine(line);
	readHeaders(ss);
}

// TODO : 情報セットする
void HTTPRequestParse::readRequestLine(std::string &line)
{
	std::cout << "--- readRequestLine ---" << std::endl;
	std::cout << line << std::endl;
}

// TODO : mapで情報セットする
void HTTPRequestParse::readHeaders(std::stringstream &ss)
{
	std::cout << "--- readHeaders ---" << std::endl;
	std::string line;
	while (getlineWithCRLF(ss, line))
	{
		std::cout << line << std::endl;
	}
}