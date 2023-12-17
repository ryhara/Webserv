#pragma once

#include <iostream>
#include <string>
#include "HTTPRequest.hpp"

class HTTPRequest;

class HTTPRequestParse
{
	private:
		HTTPRequest &_request;
	public:
		HTTPRequestParse(HTTPRequest &request);
		~HTTPRequestParse(void);

};

/* example */

// Received: GET / HTTP/1.1
// Host: localhost:4242
// Connection: keep-alive
// sec-ch-ua: "Not_A Brand";v="8", "Chromium";v="120", "Google Chrome";v="120"
// sec-ch-ua-mobile: ?0
// sec-ch-ua-platform: "macOS"
// Upgrade-Insecure-Requests: 1
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
// Sec-Fetch-Site: none
// Sec-Fetch-Mode: navigate
// Sec-Fetch-User: ?1
// Sec-Fetch-Dest: document
// Accept-Encoding: gzip, deflate, br
// Accept-Language: ja-JP,ja;q=0.9,en-US;q=0.8,en;q=0.7
// Cookie: wp-settings-time-1=1701743409


/* parse example */
// method: GET
// uri: /cgi/index.html
// protocol_version: HTTP/1.1
// message_body:
// server_config: 0x15b704080
// location: /cgi/
// --------------------------------------------------------------------
// headers:
// accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
// accept-encoding: gzip, deflate, br
// accept-language: ja-JP,ja;q=0.9,en-US;q=0.8,en;q=0.7
// connection: keep-alive
// cookie: wp-settings-time-1=1701743409
// host: localhost:4242
// sec-ch-ua: "Not_A Brand";v="8", "Chromium";v="120", "Google Chrome";v="120"
// sec-ch-ua-mobile: ?0
// sec-ch-ua-platform: "macOS"
// sec-fetch-dest: document
// sec-fetch-mode: navigate
// sec-fetch-site: none
// sec-fetch-user: ?1
// upgrade-insecure-requests: 1
// user-agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36

