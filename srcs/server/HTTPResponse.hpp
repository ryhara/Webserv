#pragma once

#include <string>
#include <iostream>
#include <map>

#include "HTTPStatusCode.hpp"

#define CRLF "\r\n"

class HTTPResponse
{
	private:
		std::string _version;
		HTTPStatusCode _statusCode;
		std::string _statusMessage;
		std::string _statusLine;
		bool keepAlive; // TODO : keepAlive 調べる
		size_t _contentLength;
		std::map<std::string, std::string> _headers;
		std::string _body;
	public :
		HTTPResponse();
		~HTTPResponse();

		// setter
		void setVersion(const std::string &version);
		void setStatusCode(const HTTPStatusCode &statusCode);
		void setStatusMessage(const std::string &statusMessage);
		void setStatusLine(const std::string &statusLine);
		void setKeepAlive(const bool &keepAlive);
		void setContentLength(const size_t &contentLength);
		void setHeader(const std::string &key, const std::string &value);
		void setBody(const std::string &body);
		// getter
		const std::string &getVersion() const;
		const HTTPStatusCode &getStatusCode() const;
		const std::string &getStatusMessage() const;
		const std::string &getStatusLine() const;
		const bool &getKeepAlive() const;
		const size_t &getContentLength() const;
		const std::map<std::string, std::string> &getHeaders() const;
		const std::string &getBody() const;
};

/* response example */
// HTTP/1.1 200 OK
// Connection: keep-alive
// Content-Length: 192
// Date: Thu, 21 Dec 2023 02:01:21 GMT
// Server: webserv/1.0.0

// <!DOCTYPE html>
// <html>

// <head>
//   <meta charset='utf-8'>
//   <title>Welcome to webserv!</title>
// </head>

// <body>
//   <h1>Welcome to webserv!</h1>
//   <p>Hello from docs/index.html</p>
// </body>