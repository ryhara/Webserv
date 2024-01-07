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

# main関数、kqueueを用いているファイルを除外(kqueueはMacOSのみで使用可能)
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/srcs/*.cpp")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/srcs/main.cpp")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/srcs/server/Server.cpp")
list(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/srcs/server/Kqueue.cpp")
file(GLOB TEST_FILES "${PROJECT_SOURCE_DIR}/tests/*_test.cpp") # テストファイルの指定
# ファイルをビルドディレクトリにコピーする
file(COPY ${PROJECT_SOURCE_DIR}/www/ DESTINATION ${CMAKE_BINARY_DIR}/www/)
file(COPY ${PROJECT_SOURCE_DIR}/config/ DESTINATION ${CMAKE_BINARY_DIR}/config/)


enable_testing()

add_executable(${PROGRAM}
	${SOURCES}
	${TEST_FILES}
)
target_link_libraries(${PROGRAM}
	gtest_main
)

target_include_directories(${PROGRAM}
	PRIVATE
		${PROJECT_SOURCE_DIR}/srcs/cgi/
		${PROJECT_SOURCE_DIR}/srcs/config/
		${PROJECT_SOURCE_DIR}/srcs/exeption/
		${PROJECT_SOURCE_DIR}/srcs/server/
		${PROJECT_SOURCE_DIR}/srcs/utils/
		${PROJECT_SOURCE_DIR}/srcs/
)
# テストの自動検出
gtest_discover_tests(${PROGRAM})