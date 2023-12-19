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
	if (!std::getline(ss, line) && line.empty())
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


void HTTPRequestParse::parse(char *buffer)
{
	std::string line;
	std::stringstream bufferStream(buffer);

	if (!getlineWithCRLF(bufferStream, line))
		log_exit("getline", __LINE__, __FILE__);
	readRequestLine(line);
	readHeaders(bufferStream);
	_request.print();
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
}

void HTTPRequestParse::readHeaders(std::stringstream &ss)
{
	std::string line;
	std::pair<std::string, std::string> pair;
	std::vector<std::string> header;
	while (getlineWithCRLF(ss, line))
	{
		if (line.empty())
			break;
		std::vector<std::string> header = split(line, std::string(": "));
		pair.first = header[0];
		pair.second = header[1];
		this->_request.setHeaders(pair);
	}
}

// TODO : /cgi/index.html -> /cgi/ みたいにlocationを探す
void HTTPRequestParse::searchLocation(void)
{
	std::string uri = this->_request.getUri();
	if (uri.compare("/") == 0)
	{
		this->_request.setLocation("/");
		return ;
	}
	std::vector<std::string> uri_split = split(uri, '/');
	std::string location = uri_split[1];
	location = "/" + location + "/";
	this->_request.setLocation(location);
}
