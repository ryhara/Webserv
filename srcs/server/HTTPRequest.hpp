#pragma once

#include <iostream>
#include <string>
#include <map>

class HTTPRequest
{
	private:
		std::string _method; // GET, POST etc...
		std::string _uri; // /, /cgi/index.html
		std::string _version; // HTTP/1.1 etc...
		std::map<std::string, std::string> _headers; // etc...
		std::string _body; // TODO : POSTなどのときに使う
		std::string _location; // TODO : /, /cgi/
	public :
		HTTPRequest(void);
		~HTTPRequest(void);

		void			clear(void);
		// getter
		std::string		&getMethod(void) const;
		std::string		&getUri(void) const;
		std::string		&getVersion(void) const;
		std::map<std::string, std::string> &getHeaders(void) const;
		std::string		&getBody(void) const;
		std::string		&getLocation(void) const;
		// setter
		void			setMethod(const std::string &method);
		void			setUri(const std::string &uri);
		void			setVersion(const std::string &version);
		void			setHeaders(const std::pair<std::string, std::string> &header);
		void			setBody(const std::string &body);
		void			setLocation(const std::string &location);

};