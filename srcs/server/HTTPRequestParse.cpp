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
	if (!line.empty() && line[line.size() - 1] == '\r')
		line.erase(line.size() - 1);
	return (1);
}

std::vector<std::string> HTTPRequestParse::split(std::string str, char del)
{
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string tmp;

	while (getline(ss, tmp, del))
	{
		result.push_back(tmp);
	}
	return (result);
}

void HTTPRequestParse::parse(char *buffer)
{
	std::string line;
	std::stringstream bufferStream(buffer);

	// TODO : 1行ずつ読み込む
	if (!getlineWithCRLF(bufferStream, line))
		log_exit("getline", __LINE__, __FILE__);
	readRequestLine(line);
	_request.print();
	readHeaders(bufferStream);
}

// TODO : 情報セットする
void HTTPRequestParse::readRequestLine(std::string &line)
{
	std::cout << "--- readRequestLine ---" << std::endl;
	std::cout << line << std::endl;
	std::vector<std::string> request_line = split(line, ' ');
	if (request_line.size() != 3)
		log_exit("request_line.size() != 3", __LINE__, __FILE__);
	this->_request.setMethod(request_line[0]);
	this->_request.setUri(request_line[1]);
	this->_request.setVersion(request_line[2]);
}

// TODO : mapで情報セットする
void HTTPRequestParse::readHeaders(std::stringstream &ss)
{
	std::cout << "--- readHeaders ---" << std::endl;
	std::string line;
	std::string key;
	std::string value;
	std::vector<std::string> header;
	while (getlineWithCRLF(ss, line))
	{
		// TODO : ": "で区切って、keyとvalueに分ける -> mapに入れる
		if (line.empty())
			break;
		std::vector<std::string> header = split(line, ':');
	}
}