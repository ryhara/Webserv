#pragma once

#include <iostream>
#include <string>
#include <map>

class HTTPRequest
{
	private:
		std::string _method;
		std::string _uri;
		std::string _version;
		std::map<std::string, std::string> _headers;
		std::string _body;
		std::string _location;
	public :
		HTTPRequest(void);
		~HTTPRequest(void);
		// getter
		void			parse(std::string request);
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