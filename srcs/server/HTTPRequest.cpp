#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest(void) : _method(""), _uri(""), _version(""), _headers(), _body(""), _location("")
{
}

HTTPRequest::~HTTPRequest(void)
{
}


void HTTPRequest::clear(void)
{
	this->_method = "";
	this->_uri = "";
	this->_version = "";
	this->_headers.clear();
	this->_body = "";
	this->_location = "";
}

// getter
std::string		&HTTPRequest::getMethod(void) const
{
	return (const_cast<std::string &>(this->_method));
}

std::string		&HTTPRequest::getUri(void) const
{
	return (const_cast<std::string &>(this->_uri));
}

std::string		&HTTPRequest::getVersion(void) const
{
	return (const_cast<std::string &>(this->_version));
}

std::map<std::string, std::string> &HTTPRequest::getHeaders(void) const
{
	return (const_cast<std::map<std::string, std::string> &>(this->_headers));
}

std::string		&HTTPRequest::getBody(void) const
{
	return (const_cast<std::string &>(this->_body));
}

std::string		&HTTPRequest::getLocation(void) const
{
	return (const_cast<std::string &>(this->_location));
}

// setter
void			HTTPRequest::setMethod(const std::string &method)
{
	this->_method = method;
}

void			HTTPRequest::setUri(const std::string &uri)
{
	this->_uri = uri;
}

void			HTTPRequest::setVersion(const std::string &version)
{
	this->_version = version;
}

void			HTTPRequest::setHeaders(const std::pair<std::string, std::string> &header)
{
	this->_headers.insert(header);
}

void			HTTPRequest::setBody(const std::string &body)
{
	this->_body = body;
}

void			HTTPRequest::setLocation(const std::string &location)
{
	this->_location = location;
}

void HTTPRequest::print(void)
{
	std::cout << "method: " << this->_method << std::endl;
	std::cout << "uri: " << this->_uri << std::endl;
	std::cout << "protocol_version: " << this->_version << std::endl;
	std::cout << "message_body: " << this->_body << std::endl;
	std::cout << "location: " << this->_location << std::endl;
	std::cout << "--------------------------------------------------------------------" << std::endl;
	std::cout << "headers: " << std::endl;
	for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
	{
		std::cout << it->first << ": " << it->second << std::endl;
	}
}
