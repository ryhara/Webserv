#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <sstream>
#include "Hello.h"

TEST(HelloTest, OutputsHello) {
	std::string name = "World";
	std::string expected = "Hello World\n";
	// 標準出力をキャプチャする
	std::stringstream buffer;
	std::streambuf* prevcoutbuf = std::cout.rdbuf(buffer.rdbuf());
	// 関数を呼び出す
	hello(name);

	// 標準出力を元に戻す
	std::cout.rdbuf(prevcoutbuf);

	// テストアサーション
	EXPECT_EQ(expected, buffer.str());
}
