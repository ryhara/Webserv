#pragma once

#include <iostream>
#include <string>

class RequestParse
{
private:
	std::string _request;
	std::string _method;
	std::string _uri;
	std::string _version;
	std::string _header;
	std::string _body;
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
