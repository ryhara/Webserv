#pragma once

#include <iostream>
#include <string>
#include <map>
#include <sys/stat.h>
#include "Webserv.hpp"
#include "ServerConfig.hpp"

class HTTPRequest
{
	private:
		std::string _method;
		std::string _uri;
		std::string _version;
		std::map<std::string, std::string> _headers;
		std::string _body;
		std::string _location;
		std::string _host;
		std::string _port;
		struct stat _stat;
		enum response_mode _mode;
		ServerConfig _config;

		DISALLOW_COPY_AND_ASSIGN(HTTPRequest);

	public :
		HTTPRequest(void);
		~HTTPRequest(void);

		void			clear(void);
		void			print(void);
		// getter
		std::string		&getMethod(void) const;
		std::string		&getUri(void) const;
		std::string		&getVersion(void) const;
		std::map<std::string, std::string> &getHeaders(void) const;
		std::string 	getHeader(const std::string &key) const;
		std::string		&getBody(void) const;
		std::string		&getLocation(void) const;
		struct stat		*getStat(void) const;
		enum response_mode getMode(void) const;
		std::string		&getHost(void) const;
		std::string		&getPort(void) const;
		ServerConfig	&getServerConfig(void);

		// setter
		void			setMethod(const std::string &method);
		void			setUri(const std::string &uri);
		void			setVersion(const std::string &version);
		void			setHeaders(const std::pair<std::string, std::string> &header);
		void			setBody(const std::string &body);
		void			addBody(const std::string &body);
		void			setLocation(const std::string &location);
		void 			setStat(const struct stat &stat);
		void			setMode(const enum response_mode mode);
		void			setHost(const std::string &host);
		void			setPort(const std::string &port);
		void			setServerConfig(ServerConfig &config);

};
