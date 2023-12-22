#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse() : _version(HTTP_VERSION), keepAlive(false), _contentLength(0)
{
	setStatusMessageMap();
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

// TODO : 初期化
void HTTPResponse::setStatusMessageMap(void)
{
	_statusMessageMap[STATUS_100] = "Continue";
	_statusMessageMap[STATUS_101] = "Switching Protocols";

	_statusMessageMap[STATUS_200] = "OK";
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