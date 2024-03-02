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
		virtual const char* what() const throw() = 0;
};

class BadRequestError : public ServerException
{
	public:
		~BadRequestError() throw() {}
		BadRequestError(void) : ServerException(STATUS_400, "Bad Request") {}
		BadRequestError(HTTPStatusCode status_code, std::string status_message) : ServerException(status_code, status_message) {}
		virtual const char *what() const throw()
		{
			return (_status_message.c_str());
		}
};

class InternalServerError : public ServerException
{
	public:
		~InternalServerError() throw() {}
		InternalServerError(void): ServerException(STATUS_500, "Internal Server Error") {}
		InternalServerError(HTTPStatusCode status_code, std::string status_message) : ServerException(status_code, status_message) {}
		virtual const char *what() const throw()
		{
			return (_status_message.c_str());
		}
};

class HTTPRequestPayloadTooLargeError: public ServerException
{
	public:
		~HTTPRequestPayloadTooLargeError() throw() {}
		HTTPRequestPayloadTooLargeError(void) : ServerException(STATUS_413, "Request Entity Too Large") {}
		HTTPRequestPayloadTooLargeError(HTTPStatusCode status_code, std::string status_message) : ServerException(status_code, status_message) {}
		virtual const char *what() const throw()
		{
			return (_status_message.c_str());
		}
};

class MethodNotAllowedError : public ServerException
{
	public:
		~MethodNotAllowedError() throw() {}
		MethodNotAllowedError(void) : ServerException(STATUS_405, "Method Not Allowed") {}
		MethodNotAllowedError(HTTPStatusCode status_code, std::string status_message) : ServerException(status_code, status_message) {}
		virtual const char *what() const throw()
		{
			return (_status_message.c_str());
		}
};

class NotImplementedError : public ServerException
{
	public:
		~NotImplementedError() throw() {}
		NotImplementedError(void) : ServerException(STATUS_501, "Not Implemented") {}
		NotImplementedError(HTTPStatusCode status_code, std::string status_message) : ServerException(status_code, status_message) {}
		virtual const char *what() const throw()
		{
			return (_status_message.c_str());
		}
};

class HTTPVersionNotSupportedError : public ServerException
{
	public:
		~HTTPVersionNotSupportedError() throw() {}
		HTTPVersionNotSupportedError(void) : ServerException(STATUS_505, "HTTP Version Not Supported") {}
		HTTPVersionNotSupportedError(HTTPStatusCode status_code, std::string status_message) : ServerException(status_code, status_message) {}
		virtual const char *what() const throw()
		{
			return (_status_message.c_str());
		}
};

class NotFoundError : public ServerException
{
	public:
		~NotFoundError() throw() {}
		NotFoundError(void) : ServerException(STATUS_404, "Not Found") {}
		NotFoundError(HTTPStatusCode status_code, std::string status_message) : ServerException(status_code, status_message) {}
		virtual const char *what() const throw()
		{
			return (_status_message.c_str());
		}
};

class ForbiddenError : public ServerException
{
	public:
		~ForbiddenError() throw() {}
		ForbiddenError(void) : ServerException(STATUS_403, "Forbidden") {}
		ForbiddenError(HTTPStatusCode status_code, std::string status_message) : ServerException(status_code, status_message) {}
		virtual const char *what() const throw()
		{
			return (_status_message.c_str());
		}
};