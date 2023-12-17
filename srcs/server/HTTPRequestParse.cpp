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