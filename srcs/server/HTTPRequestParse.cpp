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
				#if DEBUG
					std::cout << "#### [ DEBUG ] REQUEST_LINE_STATE ####" << std::endl;
				#endif
				_requiredParse = readRequestLine(bufferStream);
				setHTTPRequestParseState(HEADERS_STATE);
				break;
			case HEADERS_STATE:
				#if DEBUG
					std::cout << "#### [ DEBUG ] HEADERS_STATE ####" << std::endl;
				#endif
				_requiredParse=  readHeaders(bufferStream);
				break;
			case BODY_STATE:
				#if DEBUG
					std::cout << "#### [ DEBUG ] BODY_STATE ####" << std::endl;
					std::cout << _request.getBody() << " " << _request.getBody().size() << std::endl;
				#endif
				_requiredParse = readBody(bufferStream);
				// TODO : telnetでPOST対応するならbodyの読み方変える。
				setHTTPRequestParseState(FINISH_STATE);
				break;
			case FINISH_STATE:
				#if DEBUG
					std::cout << "#### [ DEBUG ] FINISH_STATE ####" << std::endl;
				#endif
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

	// getlineWithCRLF(bufferStream, line);
	// if (line.empty()) {
	// 	throw HTTPRequestParseError();
	// 	return ;
	// }
	// readRequestLine(line);
	// readHeaders(bufferStream);
	// readBody(bufferStream);
	// #if DEBUG
	// 	_request.print();
	// # endif
	// bufferStream.clear();
	// bufferStream.str("");
}

bool HTTPRequestParse::readRequestLine(std::stringstream &ss)
{
	std::string line = "";
	if (!getlineWithCRLF(ss, line)) {
		return (false);
	}
	std::vector<std::string> request_line = split(line, ' ');
	if (request_line.size() != 3)
		throw HTTPRequestParseError();
	// TODO : methodが対応してない場合にエラー
	if (request_line[2].compare(HTTP_VERSION) != 0)
		throw HTTPVersionNotSupportedError();
	this->_request.setMethod(request_line[0]);
	this->_request.setUri(request_line[1]);
	this->_request.setVersion(request_line[2]);
	searchLocation();
	searchRequestMode();
	return (true);
}

bool HTTPRequestParse::readHeaders(std::stringstream &ss)
{
	std::string line = "";
	std::pair<std::string, std::string> pair;
	std::vector<std::string> header;
	if (getlineWithCRLF(ss, line))
	{
		if (line.empty() && _contentLength) {
			setHTTPRequestParseState(BODY_STATE);
			return (true);
		} else if (line.empty()) {
			setHTTPRequestParseState(FINISH_STATE);
			return (true);
		}
		std::vector<std::string> header = split(line, std::string(": "));
		if (header.size() != 2)
			throw HTTPRequestParseError();
		pair.first = header[0];
		if (header[0].compare("Transfer-Encoding") == 0 && header[1].compare("chunked") == 0)
			this->_isChunked = true;
		if (header[0].compare("Content-Length") == 0) {
			this->_contentLength = ft_stoi(header[1]);
		}
		pair.second = header[1];
		this->_request.setHeaders(pair);
		return (true);
	} else {
		return (false);
	}
}

void HTTPRequestParse::parseChunkedBody(std::stringstream &ss)
{
	std::string line, body;
	size_t count = 0;
	int size;
	while (getlineWithCRLF(ss, line))
	{
		if (line.empty())
			break;
		if (count % 2 == 0) {
			if (!isHex(line))
				throw HTTPRequestParseError();
			std::stringstream ss(line);
			ss >> std::hex >> size;
			if (size == 0)
				break;
			ss.clear();
			ss.str("");
		} else {
			body += line;
		}
		count++;
	}
	if (!body.empty())
		this->_request.setBody(body);
	if (body.size() > MAX_BODY_SIZE)
		throw HTTPRequestPayloadTooLargeError();
}

void HTTPRequestParse::parseNormalBody(std::stringstream &ss)
{
	std::string body;
	std::string line = "";
	while (getlineWithCRLF(ss, line))
	{
		if (line.empty())
			break;
		body += line;
	}
	if (!body.empty())
		this->_request.setBody(body);
	if (body.size() > MAX_BODY_SIZE)
		throw HTTPRequestPayloadTooLargeError();
}

bool HTTPRequestParse::readBody(std::stringstream &ss)
{
	if (this->_isChunked)
	{
		parseChunkedBody(ss);
	} else {
		parseNormalBody(ss);
	}
	return (true);
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