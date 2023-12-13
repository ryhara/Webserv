#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <sstream>
#include "test.hpp"

// TEST(テストケース名, テスト名)
TEST(FunctionTest, HelloTestFunction) {
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

TEST(FunctionTest, AddTestFunction) {
    EXPECT_EQ(3, add_test(1, 2));
    EXPECT_EQ(5, add_test(2, 3));
    EXPECT_EQ(0, add_test(-1, 1));
}