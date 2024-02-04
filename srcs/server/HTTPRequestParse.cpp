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
		if (!tmp.empty())
			result.push_back(tmp);
	}
	return (result);
}

std::vector<std::string> HTTPRequestParse::split(std::string str, std::string del)
{
	std::vector<std::string> result;
	std::string::size_type pos = str.find(del);
	std::string::size_type del_size = del.size();

	if (pos == std::string::npos)
		return (result);
	result.push_back(str.substr(0, pos));
	result.push_back(str.substr(pos + del_size));
	return (result);
}

// TODO : parse失敗時にはexitではなく、例外を投げる（ステータスコード400など）、プログラム的なエラーはexit,https通信の場合は、parseできないので、例外を投げる
void HTTPRequestParse::parse(char *buffer)
{
	std::string line;
	std::stringstream bufferStream(buffer);

	getlineWithCRLF(bufferStream, line);
	if (line.empty()) {
		// TODO : error message
		return ;
	}
	readRequestLine(line);
	readHeaders(bufferStream);
	_request.print();
	bufferStream.clear();
	bufferStream.str("");
}

void HTTPRequestParse::readRequestLine(std::string &line)
{
	std::vector<std::string> request_line = split(line, ' ');
	if (request_line.size() != 3)
		log_exit("request_line.size() != 3", __LINE__, __FILE__);
	this->_request.setMethod(request_line[0]);
	this->_request.setUri(request_line[1]);
	this->_request.setVersion(request_line[2]);
	searchLocation();
	searchRequestMode();
}

void HTTPRequestParse::readHeaders(std::stringstream &ss)
{
	std::string line;
	std::pair<std::string, std::string> pair;
	std::vector<std::string> header;
	std::string body;
	while (getlineWithCRLF(ss, line))
	{
		if (line.empty())
			break;
		std::vector<std::string> header = split(line, std::string(": "));
		pair.first = header[0];
		pair.second = header[1];
		this->_request.setHeaders(pair);
	}
	while (getlineWithCRLF(ss, line))
	{
		if (line.empty())
			break;
		body += line;
	}
	if (!body.empty())
		this->_request.setBody(body);
}

void HTTPRequestParse::searchLocation(void)
{
	std::string uri = this->_request.getUri();
	if (uri.compare("/") == 0)
	{
		this->_request.setLocation("/");
		return ;
	}
	std::vector<std::string> uri_split = split(uri, '/');
	if (uri_split.size() < 2)
	{
		this->_request.setLocation("/");
		return ;
	}
	std::string location = uri_split[0];
	location = "/" + location + "/";
	this->_request.setLocation(location);
}

void HTTPRequestParse::searchRequestMode(void)
{
	std::string location = this->_request.getLocation();
	if (location.compare("/cgi/") == 0) {
		this->_request.setMode(CGI);
	} else if (location.compare("/autoindex/") == 0) {
		this->_request.setMode(AUTOINDEX);
	} else if (location.compare("/redirect/") == 0) {
		this->_request.setMode(REDIRECT);
	} else {
		this->_request.setMode(NORMAL);
	}
}