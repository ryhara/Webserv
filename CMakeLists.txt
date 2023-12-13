cmake_minimum_required(VERSION 3.14)
project(webserv VERSION 1.0 LANGUAGES CXX)

# C++の標準を設定
set(CMAKE_CXX_STANDARD 98)

set(PROGRAM webserv)


# Google Testをダウンロードして統合
include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/release-1.12.1.tar.gz
	DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)
# Google Testのモジュールをインクルード
include(GoogleTest)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/srcs/*.cpp")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/srcs/main.cpp") # メイン関数を含むファイルを除外
file(GLOB TEST_FILES "${PROJECT_SOURCE_DIR}/tests/*_test.cpp") # テストファイルの指定

enable_testing()

add_executable(${PROGRAM}
	${SOURCES}
	${TEST_FILES}
)
target_link_libraries(${PROGRAM}
	gtest_main
)

# インクルードディレクトリの指定
target_include_directories(${PROGRAM}
	PRIVATE
	${PROJECT_SOURCE_DIR}/include
)