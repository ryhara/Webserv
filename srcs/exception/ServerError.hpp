#include <exception>

class HTTPRequestParseError : public std::exception
{
	public:
		virtual const char *what() const throw()
		{
			return ("HTTP Request Parse Error");
		}
};

class InternalServerError : public std::exception
{
	public:
		virtual const char *what() const throw()
		{
			return ("Internal Server Error");
		}
};
