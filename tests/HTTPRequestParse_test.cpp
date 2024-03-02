#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <sstream>
#include "../srcs/server/HTTPRequestParse.hpp"
#include "SurpressOutput.hpp"

char example[] = "GET /cgi/index.html HTTP/1.1\r\nHost: localhost:4242\r\nConnection: keep-alive\r\nsec-ch-ua: \"Not_A Brand\";v=\"8\", \"Chromium\";v=\"120\", \"Google Chrome\";v=\"120\"\r\n";

TEST(HTTPRequestParseTest, readRequestLineTest) {
	// テスト時に標準出力を無視させる
	SurpressOutput surpressOutput;

	HTTPRequest request;
	HTTPRequestParse parse(request);
	std::stringstream ss("GET / HTTP/1.1\r\n");
	parse.readRequestLine(ss);
	EXPECT_EQ(request.getMethod(), "GET");
	EXPECT_EQ(request.getUri(), "/");
	EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(HTTPRequestParseTest, readHeadersTest) {
	SurpressOutput surpressOutput;

	char example1[] = "Host: localhost:4242\r\nConnection: keep-alive\r\n";
	HTTPRequest request;
	HTTPRequestParse parse(request);
	std::stringstream ss(example1);
	parse.readHeaders(ss);
	parse.readHeaders(ss);
	EXPECT_EQ(request.getHeaders()["Host"], "localhost:4242");
	EXPECT_EQ(request.getHeaders()["Connection"], "keep-alive");
}

TEST(HTTPRequestParseTest, parseTest1) {
	SurpressOutput surpressOutput;

	char example2[] = "GET / HTTP/1.1\r\nHost: localhost:4242\r\nConnection: keep-alive\r\n";
	HTTPRequest request;
	HTTPRequestParse parse(request);
	parse.parse(example2);
	EXPECT_EQ(request.getMethod(), "GET");
	EXPECT_EQ(request.getUri(), "/");
	EXPECT_EQ(request.getVersion(), "HTTP/1.1");
}

TEST(HTTPRequestParseTest, parseTest2) {
	SurpressOutput surpressOutput;

	HTTPRequest request;
	HTTPRequestParse parse(request);
	parse.parse(example);
	EXPECT_EQ(request.getMethod(), "GET");
	EXPECT_EQ(request.getUri(), "/cgi/index.html");
	EXPECT_EQ(request.getVersion(), "HTTP/1.1");
	EXPECT_EQ(request.getHeaders()["Host"], "localhost:4242");
	EXPECT_EQ(request.getHeaders()["Connection"], "keep-alive");
	EXPECT_EQ(request.getHeaders()["sec-ch-ua"], "\"Not_A Brand\";v=\"8\", \"Chromium\";v=\"120\", \"Google Chrome\";v=\"120\"");
}

TEST(HTTPRequestParseTest, locationTest) {
	SurpressOutput surpressOutput;

	HTTPRequest request;
	HTTPRequestParse parse(request);
	parse.parse(example);
	EXPECT_EQ(request.getLocation(), "/cgi/");
}