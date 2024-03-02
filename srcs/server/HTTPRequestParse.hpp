#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "HTTPRequest.hpp"
#include "HTTPStatusCode.hpp"
#include "utils.hpp"
#include "Webserv.hpp"

class HTTPRequest;

enum HTTPRequestParseState
{
	REQUEST_LINE_STATE,
	HEADERS_STATE,
	BODY_STATE,
	FINISH_STATE,
};

class HTTPRequestParse
{
	private:
		HTTPRequest &_request;
		bool _requiredParse;
		bool _requiredBodySize;
		bool _isChunked;
		size_t _contentLength;
		HTTPRequestParseState _state;
		DISALLOW_COPY_AND_ASSIGN(HTTPRequestParse);
	public:
		HTTPRequestParse(HTTPRequest &request);
		~HTTPRequestParse(void);
		void clear(void);

		HTTPRequest &getRequest(void) const;
		void parse(char *buffer);
		std::vector<std::string> split(std::string str, char del);
		std::vector<std::string> split(std::string str, std::string del);
		int countSpace(const std::string &str);
		bool readRequestLine(std::stringstream &ss);
		bool readHeaders(std::stringstream &ss);
		bool readBody(std::stringstream &ss);
		void searchLocation(void);
		void searchRequestMode(void);
		int getlineWithCRLF(std::stringstream &ss, std::string &line);
		void parseChunkedBody(std::stringstream &ss);
		void parseNormalBody(std::stringstream &ss);

		HTTPRequestParseState getHTTPRequestParseState(void) const;
		void setHTTPRequestParseState(HTTPRequestParseState state);
};
