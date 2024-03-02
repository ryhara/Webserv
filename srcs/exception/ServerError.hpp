#pragma once
#include <exception>
#include <string>

#include "HTTPStatusCode.hpp"

class ServerException : public std::exception
{
	protected:
		HTTPStatusCode _status_code;
		std::string _status_message;
	public:
		ServerException(HTTPStatusCode status_code, std::string status_message) : _status_code(status_code), _status_message(status_message) {}
		virtual ~ServerException() throw() {}
		HTTPStatusCode getStatusCode(void) const
		{
			return (_status_code);
		}
		virtual const char *what() const throw()
		{
			return (_status_message.c_str());
		}
};
