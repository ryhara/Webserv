cmake_minimum_required(VERSION 3.14)
project(webserv VERSION 1.0 LANGUAGES CXX)

# C++の標準を設定
set(CMAKE_CXX_STANDARD 98)

# ソースファイルを指定
file(GLOB_RECURSE SOURCE_FILES "src/**/*.cpp")

# 実行ファイルを追加
add_executable(webserv ${SOURCE_FILES})

# Google Testをダウンロードして統合
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.10.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

# テスト実行ファイルを追加
file(GLOB_RECURSE TEST_FILES "tests/*.cpp")
add_executable(example_test ${TEST_FILES})
target_link_libraries(example_test gtest_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(example_test)
