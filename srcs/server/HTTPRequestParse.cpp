#include "HTTPRequestParse.hpp"

HTTPRequestParse::HTTPRequestParse(HTTPRequest &request) : _request(request) , _requiredParse(true), _isChunked(false), _contentLength(0), _state(REQUEST_LINE_STATE)
{
}

HTTPRequestParse::~HTTPRequestParse(void)
{
}

void HTTPRequestParse::clear(void)
{
	this->_requiredParse = true;
	this->_isChunked = false;
	this->_contentLength = 0;
	this->_state = REQUEST_LINE_STATE;
}

HTTPRequest &HTTPRequestParse::getRequest(void) const
{
	return (this->_request);
}

HTTPRequestParseState HTTPRequestParse::getHTTPRequestParseState(void) const
{
	return (this->_state);
}

void HTTPRequestParse::setHTTPRequestParseState(HTTPRequestParseState state)
{
	this->_state = state;
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

int HTTPRequestParse::countSpace(const std::string &str)
{
	int count = 0;
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == ' ')
			count++;
	}
	return (count);
}

void HTTPRequestParse::parse(char *buffer)
{
	std::string line;
	std::stringstream bufferStream(buffer);

	if (_request.getMethod().empty()) {
		setHTTPRequestParseState(REQUEST_LINE_STATE);
	}
	while (_requiredParse)
	{
		switch (_state)
		{
			case REQUEST_LINE_STATE:
				// #if DEBUG
				// 	std::cout << "#### [ DEBUG ] REQUEST_LINE_STATE ####" << std::endl;
				// #endif
				_requiredParse = readRequestLine(bufferStream);
				break;
			case HEADERS_STATE:
				// #if DEBUG
				// 	std::cout << "#### [ DEBUG ] HEADERS_STATE ####" << std::endl;
				// #endif
				_requiredParse=  readHeaders(bufferStream);
				break;
			case BODY_STATE:
				_requiredParse = readBody(bufferStream);
				break;
			case FINISH_STATE:
				// #if DEBUG
				// 	std::cout << "#### [ DEBUG ] FINISH_STATE ####" << std::endl;
				// #endif
				if (_request.getHeader("Host").empty())
					throw ServerException(STATUS_400, "Bad Request");
				if (_contentLength > 0 && _request.getBody().size() != _contentLength)
					throw ServerException(STATUS_400, "Bad Request");
				_requiredParse = false;
				break;
		}
	}
	_requiredParse = true;
	#if DEBUG
		_request.print();
	# endif
	bufferStream.clear();
	bufferStream.str("");
}

bool HTTPRequestParse::readRequestLine(std::stringstream &ss)
{
	std::string line = "";
	if (!getlineWithCRLF(ss, line)) {
		return (false);
	}
	if (line.empty()) {
		return (false);
	}
	std::vector<std::string> request_line = split(line, ' ');
	if (countSpace(line) != 2)
		throw ServerException(STATUS_400, "Bad Request");
	if (request_line.size() != 3)
		throw ServerException(STATUS_400, "Bad Request");
	if (request_line[0].compare("GET") != 0 && request_line[0].compare("POST") != 0 && request_line[0].compare("DELETE") != 0)
		throw ServerException(STATUS_501, "Not Implemented");
	if (request_line[2].compare(HTTP_VERSION) != 0)
		throw ServerException(STATUS_505, "HTTP Version Not Supported");
	if (request_line[1][0] != '/' || request_line[1].find("/../") != std::string::npos)
		throw ServerException(STATUS_400, "Bad Request");
	this->_request.setMethod(request_line[0]);
	this->_request.setUri(request_line[1]);
	this->_request.setVersion(request_line[2]);
	searchLocation();
	if (!_request.getMethod().empty())
		setHTTPRequestParseState(HEADERS_STATE);
	return (true);
}

bool HTTPRequestParse::readHeaders(std::stringstream &ss)
{
	std::string line = "";
	std::pair<std::string, std::string> pair;
	std::vector<std::string> header;
	if (getlineWithCRLF(ss, line))
	{
		if (line.empty() && (_contentLength > 0 || _isChunked)) {
			setHTTPRequestParseState(BODY_STATE);
			return (true);
		} else if (line.empty()) {
			setHTTPRequestParseState(FINISH_STATE);
			return (true);
		}
		std::vector<std::string> header = split(line, std::string(": "));
		if (header.size() != 2)
			throw ServerException(STATUS_400, "Bad Request");
		pair.first = header[0];
		if (header[0].compare("Transfer-Encoding") == 0 && header[1].compare("chunked") == 0)
			this->_isChunked = true;
		if (header[0].compare("Content-Length") == 0) {
			if (!ft_isdigit(header[1]))
				throw ServerException(STATUS_400, "Bad Request");
			int length = ft_stoi(header[1]);
			if (length < 0)
				throw ServerException(STATUS_400, "Bad Request");
			this->_contentLength = length;
		}
		if (header[0].compare("Host") == 0) {
			std::vector<std::string> host = split(header[1], ':');
			if (host.size() == 2) {
				this->_request.setHost(host[0]);
				if (ft_stoi(host[1]) < 0 || ft_stoi(host[1]) > 65535)
					throw ServerException(STATUS_400, "Bad Request");
				this->_request.setPort(host[1]);
			} else if (host.size() == 1) {
				this->_request.setHost(host[0]);
				this->_request.setPort("80");
			} else {
				throw ServerException(STATUS_400, "Bad Request");
			}
			if (host.size() == 1)
				pair.second = host[0];
			else
				pair.second = header[1];
		} else {
			pair.second = header[1];
		}
		this->_request.setHeaders(pair);
		if (!_request.getHeader("Transfer-Encoding").empty() && !_request.getHeader("Content-Length").empty())
			throw ServerException(STATUS_400, "Bad Request");
		return (true);
	} else {
		if (_contentLength > 0 || _isChunked) {
			setHTTPRequestParseState(BODY_STATE);
			return (false);
		}
		else {
			return (false);
		}
	}
}

// 1回目はサイズから読み込む
// 次の行がサイズと一致しなかったらエラー
void HTTPRequestParse::parseChunkedBody(std::stringstream &ss)
{
	std::string line = "";
	static size_t size = 0;
	static bool isSize = false;
	while(getlineWithCRLF(ss, line))
	{
		if (!isSize) {
			if (line.empty()) {
				// std::cout << "File" << __FILE__ << "Line" << __LINE__ << std::endl;
				return ;
			}
			if (!isHex(line)) {
				isSize = false;
				throw ServerException(STATUS_400, "Bad Request");
			}
			std::stringstream ss(line);
			ss >> std::hex >> size;
			// std::cout << "size: " << size << std::endl;
			if (size == 0)  {
				ss.clear();
				ss.str("");
				setHTTPRequestParseState(FINISH_STATE);
				return ;
			}
			isSize = true;
		}
		else if (isSize) {
			// std::cout << "line: " << line << std::endl;
			if (line.size() != size) {
				isSize = false;
				throw ServerException(STATUS_400, "Bad Request");
			}
			_request.addBody(line);
			isSize = false;
		}
	}

}

void HTTPRequestParse::parseNormalBody(std::stringstream &ss)
{
	std::string line = "";
	if (getlineWithCRLF(ss, line))
	{
		if (line.empty())
			return ;
		_request.addBody(line);
	}
	if (_request.getBody().size() > _contentLength)
		throw ServerException(STATUS_413, "Request Entity Too Large");
	if (_request.getBody().size() == _contentLength)
		setHTTPRequestParseState(FINISH_STATE);
}

bool HTTPRequestParse::readBody(std::stringstream &ss)
{
	if (this->_isChunked)
	{
		// #if DEBUG
		// 	std::cout << "#### [ DEBUG ] CHUNKED_STATE ####" << std::endl;
		// #endif
		parseChunkedBody(ss);
	} else {
		// #if DEBUG
		// 	std::cout << "#### [ DEBUG ] BODY_STATE ####" << std::endl;
		// #endif
		parseNormalBody(ss);
	}
	if (getHTTPRequestParseState() == FINISH_STATE)
		return (true);
	return (false);
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
	if (uri_split.size() < 2 && uri[uri.size() - 1] != '/')
	{
		this->_request.setLocation("/");
		return ;
	}
	std::string location = uri_split[0];
	location = "/" + location + "/";
	this->_request.setLocation(location);
}
