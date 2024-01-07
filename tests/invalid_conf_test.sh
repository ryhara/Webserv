#!/bin/bash

# ディレクトリ名を引数として受け取る
directory=$1

# 引数で指定されたディレクトリが存在しない場合、スクリプトを終了する
if [ ! -d "$directory" ]; then
	echo -e "\033[31mDirectory does not exist: $directory\033[0m"
	exit 1
fi

# ディレクトリ内の各ファイルに対してループ処理を行う
for file in "$directory"/*
do
	if [[ ! -f "./webserv" ]]; then
		echo -e "\033[31mError: ./webserv: No such file or directory.\033[0m"
		exit 1
	fi
	if [[ -f "$file" ]]; then
		echo "Testing file: $file"
		./webserv "$file" || echo -e "\033[31mError occurred with file: $file\033[0m\n"
	fi
done
