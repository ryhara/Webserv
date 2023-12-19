#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <sstream>
#include "../srcs/server/HTTPRequestParse.hpp"

TEST(HTTPRequestParseTest, readRequestLineTest) {
	HTTPRequest request;
	HTTPRequestParse parse(request);
	std::string line = "GET / HTTP/1.1";
	parse.readRequestLine(line);
	EXPECT_EQ(request.getMethod(), "GET");
	EXPECT_EQ(request.getUri(), "/");
	EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(HTTPRequestParseTest, readHeadersTest) {
	char example[] = "Host: localhost:4242\r\nConnection: keep-alive\r\n";
	HTTPRequest request;
	HTTPRequestParse parse(request);
	std::stringstream ss(example);
	parse.readHeaders(ss);
	EXPECT_EQ(request.getHeaders()["Host"], "localhost:4242");
	EXPECT_EQ(request.getHeaders()["Connection"], "keep-alive");
	// TODO : 数を増やしてみる
	// TODO : bodyのテストも追加
}

TEST(HTTPRequestParseTest, parseTest1) {
	char example[] = "GET / HTTP/1.1\r\nHost: localhost:4242\r\nConnection: keep-alive\r\n";
	HTTPRequest request;
	HTTPRequestParse parse(request);
	parse.parse(example);
	EXPECT_EQ(request.getMethod(), "GET");
	EXPECT_EQ(request.getUri(), "/");
	EXPECT_EQ(request.getVersion(), "HTTP/1.1");
	// TODO : その他の変数のテストも追加
}

TEST(HTTPRequestParseTest, parseTest2) {
	char example[] = "GET /cgi/index.html HTTP/1.1\r\nHost: localhost:4242\r\nConnection: keep-alive\r\n";
	HTTPRequest request;
	HTTPRequestParse parse(request);
	parse.parse(example);
	EXPECT_EQ(request.getMethod(), "GET");
	EXPECT_EQ(request.getUri(), "/cgi/index.html");
	EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}