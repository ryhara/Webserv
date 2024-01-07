#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse() : _version(HTTP_VERSION), _statusCode(STATUS_200), keepAlive(true), _contentLength(0)
{
	setStatusMessageMap();
	setStatusMessage(_statusMessageMap[_statusCode]);
}

HTTPResponse::~HTTPResponse()
{
	_headers.clear();
	_statusMessageMap.clear();
}

// getter
const std::string &HTTPResponse::getVersion() const
{
	return this->_version;
}

const HTTPStatusCode &HTTPResponse::getStatusCode() const
{
	return this->_statusCode;
}

const std::string &HTTPResponse::getStatusMessage() const
{
	return this->_statusMessage;
}

const std::string &HTTPResponse::getStatusLine() const
{
	return this->_statusLine;
}

const bool &HTTPResponse::getKeepAlive() const
{
	return this->keepAlive;
}

const size_t &HTTPResponse::getContentLength() const
{
	return this->_contentLength;
}

const std::map<std::string, std::string> &HTTPResponse::getHeaders() const
{
	return this->_headers;
}

const std::string &HTTPResponse::getBody() const
{
	return this->_body;
}

const std::string &HTTPResponse::getStatusMessageFromMap(const HTTPStatusCode &statusCode) const
{
	return this->_statusMessageMap.at(statusCode);
}

// setter
void HTTPResponse::setVersion(const std::string &version)
{
	this->_version = version;
}

void HTTPResponse::setStatusCode(const HTTPStatusCode &statusCode)
{
	this->_statusCode = statusCode;
}

void HTTPResponse::setStatusMessage(const std::string &statusMessage)
{
	this->_statusMessage = statusMessage;
}

void HTTPResponse::setStatusLine(void)
{
	this->_statusLine = this->_version + SP + std::to_string(this->_statusCode) + SP + this->_statusMessage + CRLF;
}

void HTTPResponse::setKeepAlive(const bool &keepAlive)
{
	this->keepAlive = keepAlive;
}

void HTTPResponse::setContentLength(const size_t &contentLength)
{
	this->_contentLength = contentLength;
}

void HTTPResponse::setHeader(const std::string &key, const std::string &value)
{
	this->_headers[key] = value;
}

void HTTPResponse::setBody(const std::string &body)
{
	this->_body = body;
}

void SetBodyAll(void)
{

}


void HTTPResponse::setStatusMessageMap(void)
{
	_statusMessageMap[STATUS_100] = "Continue";
	_statusMessageMap[STATUS_101] = "Switching Protocols";
	_statusMessageMap[STATUS_102] = "Processing";
	_statusMessageMap[STATUS_103] = "Early Hints";
	_statusMessageMap[STATUS_200] = "OK";
	_statusMessageMap[STATUS_201] = "Created";
	_statusMessageMap[STATUS_202] = "Accepted";
	_statusMessageMap[STATUS_203] = "Non-Authoritative Information";
	_statusMessageMap[STATUS_204] = "No Content";
	_statusMessageMap[STATUS_205] = "Reset Content";
	_statusMessageMap[STATUS_206] = "Partial Content";
	_statusMessageMap[STATUS_207] = "Multi-Status";
	_statusMessageMap[STATUS_208] = "Already Reported";
	_statusMessageMap[STATUS_226] = "IM Used";
	_statusMessageMap[STATUS_300] = "Multiple Choice";
	_statusMessageMap[STATUS_301] = "Moved Permanently";
	_statusMessageMap[STATUS_302] = "Found";
	_statusMessageMap[STATUS_303] = "See Other";
	_statusMessageMap[STATUS_304] = "Not Modified";
	_statusMessageMap[STATUS_305] = "Use Proxy (not recommended)";
	_statusMessageMap[STATUS_306] = "(unused)";
	_statusMessageMap[STATUS_307] = "Temporary Redirect";
	_statusMessageMap[STATUS_308] = "Permanent Redirect";
	_statusMessageMap[STATUS_400] = "Bad Request";
	_statusMessageMap[STATUS_401] = "Unauthorized";
	_statusMessageMap[STATUS_402] = "Payment Required";
	_statusMessageMap[STATUS_403] = "Forbidden";
	_statusMessageMap[STATUS_404] = "Not Found";
	_statusMessageMap[STATUS_405] = "Method Not Allowed";
	_statusMessageMap[STATUS_406] = "Not Acceptable";
	_statusMessageMap[STATUS_407] = "Proxy Authentication Required";
	_statusMessageMap[STATUS_408] = "Request Timeout";
	_statusMessageMap[STATUS_409] = "Conflict";
	_statusMessageMap[STATUS_410] = "Gone";
	_statusMessageMap[STATUS_411] = "Length Required";
	_statusMessageMap[STATUS_412] = "Precondition Failed";
	_statusMessageMap[STATUS_413] = "Payload Too Large";
	_statusMessageMap[STATUS_414] = "URI Too Long";
	_statusMessageMap[STATUS_415] = "Unsupported Media Type";
	_statusMessageMap[STATUS_416] = "Range Not Satisfiable";
	_statusMessageMap[STATUS_417] = "Expectation Failed";
	_statusMessageMap[STATUS_418] = "I'm a teapot";
	_statusMessageMap[STATUS_421] = "Misdirected Request";
	_statusMessageMap[STATUS_422] = "Unprocessable Entity";
	_statusMessageMap[STATUS_423] = "Locked";
	_statusMessageMap[STATUS_424] = "Failed Dependency";
	_statusMessageMap[STATUS_425] = "Too Early";
	_statusMessageMap[STATUS_426] = "Upgrade Required";
	_statusMessageMap[STATUS_428] = "Precondition Required";
	_statusMessageMap[STATUS_429] = "Too Many Requests";
	_statusMessageMap[STATUS_431] = "Request Header Fields Too Large";
	_statusMessageMap[STATUS_451] = "Unavailable For Legal Reasons";
	_statusMessageMap[STATUS_500] = "Internal Server Error";
	_statusMessageMap[STATUS_501] = "Not Implemented";
	_statusMessageMap[STATUS_502] = "Bad Gateway";
	_statusMessageMap[STATUS_503] = "Service Unavailable";
	_statusMessageMap[STATUS_504] = "Gateway Timeout";
	_statusMessageMap[STATUS_505] = "HTTP Version Not Supported";
	_statusMessageMap[STATUS_506] = "Variant Also Negotiates";
	_statusMessageMap[STATUS_507] = "Insufficient Storage";
	_statusMessageMap[STATUS_508] = "Loop Detected";
	_statusMessageMap[STATUS_510] = "Not Extended";
	_statusMessageMap[STATUS_511] = "Network Authentication Required";
}


std::string HTTPResponse::getDateTimestamp(void) const
{
	std::string ret;
	time_t now = std::time(NULL);
	struct tm *gmt = std::gmtime(&now);
	char buf[DATE_BUF_SIZE] = {0};

	std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", gmt);
	ret = buf;
	return ret;
}

void HTTPResponse::makeGetResponseBody(HTTPRequest &request)
{
	std::string responseMessage;
	std::string path = request.getUri();
	std::ifstream ifs;
	// TODO : configの設定によって、pathを変更する
	ifs.open("./www" + path);
	if (!ifs)
	{
		// TODO : 404 Not Found
		responseMessage = "HTTP/1.1 404 Not Found\r\n";
	}
	else
	{
		std::string line;
		while (getline(ifs, line))
		{
			_body += line;
			if (ifs.peek() != EOF)
				_body += CRLF;
		}
	}
	ifs.close();
}

void HTTPResponse::makePostResponseBody(HTTPRequest &request)
{
	std::string responseMessage;
	std::string path = request.getUri();
}

void HTTPResponse::makeDeleteResponseBody(HTTPRequest &request)
{
	std::string responseMessage;
	std::string path = request.getUri();
}

std::string HTTPResponse::makeResponseMessage(HTTPRequest &request)
{
	std::ifstream ifs;
	std::string responseMessage;
	std::string method = request.getMethod();
	if (method == "GET")
		makeGetResponseBody(request);
	else if (method == "POST")
		makePostResponseBody(request);
	else if (method == "DELETE")
		makeDeleteResponseBody(request);
	else
		responseMessage = "HTTP/1.1 501 Not Implemented\r\n";
	// TODO : 構造の見直し
	setStatusLine();
	if (keepAlive)
		setHeader("Connection", "keep-alive");
	else
		setHeader("Connection", "close");
	setHeader("Date", getDateTimestamp());
	setHeader("Server", SERVER_NAME);
	setContentLength(_body.size());
	setHeader("Content-Length", std::to_string(_contentLength));
	responseMessage += _statusLine;
	for (std::map<std::string, std::string>::iterator it = _headers.begin(); it != _headers.end(); it++)
	{
		responseMessage += it->first + ": " + it->second + CRLF;
	}
	responseMessage += CRLF;
	responseMessage += _body;
	return responseMessage;
}