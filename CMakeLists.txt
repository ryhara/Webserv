cmake_minimum_required(VERSION 3.14)
project(webserv VERSION 1.0 LANGUAGES CXX)

# C++の標準を設定
set(CMAKE_CXX_STANDARD 98)

# ソースファイルを指定
file(GLOB_RECURSE SOURCE_FILES "src/**/*.cpp")

# Google Testをダウンロードして統合
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)
# Google Testのモジュールをインクルード
include(GoogleTest)

enable_testing()

# 各テストファイルに対して個別のテスト実行ファイルを生成
file(GLOB_RECURSE TEST_FILES "tests/*.cpp")
foreach(TEST_FILE ${TEST_FILES})
  get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_FILE})
  target_link_libraries(${TEST_NAME} gtest_main)
  gtest_discover_tests(${TEST_NAME})
endforeach()
