#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <sstream>
#include "../srcs/server/HTTPRequest.hpp"
#include "../srcs/server/HTTPRequestParse.hpp"
#include "../srcs/server/HTTPResponse.hpp"
#include "SurpressOutput.hpp"
#include "ReadFile.hpp"

HTTPRequest request;

// testケース作成
void setRequest()
{
	request.setMethod("GET");
	request.setUri("/cgi/index.html");
	request.setVersion("HTTP/1.1");
	request.setHeaders(std::make_pair("Host", "localhost:4242"));
	request.setHeaders(std::make_pair("Connection", "keep-alive"));
	request.setLocation("/cgi/");
}

// Dateは毎回変わるため、test時にDateヘッダを削除する
void removeDate(std::string& str) {
    std::string pattern = "Date";
    size_t startPos = str.find(pattern);
    if (startPos != std::string::npos) {
        size_t endPos = str.find("\r\n", startPos);
        if (endPos != std::string::npos) {
            // endPosを含む範囲を削除するために、CRLFの長さを加算
            str.erase(startPos, endPos - startPos + 2);
        }
    }
}

TEST(HTTPResponseTEST, handleNormalRequestTest) {
	ReadFile readFile("./www/cgi/index.html");
	std::string statusLine = "HTTP/1.1 200 OK\r\n";
	std::string body = readFile.getBody();
	std::string headers = "Connection: keep-alive\r\nContent-Length: " + ft_stoi(body.length()) + "\r\n" + "Server: webserv/1.0\r\n\r\n";
	std::string answer = statusLine + headers + body;
	SurpressOutput surpressOutput;
	setRequest();

	HTTPResponse response;
	std::string responseMessage;
	response.handleNormalRequest(request);
	responseMessage = response.getResponseMessage();
	removeDate(responseMessage);
	EXPECT_EQ(responseMessage, answer);
}