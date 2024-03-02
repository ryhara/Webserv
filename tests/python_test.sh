#!/bin/bash

directory=$1

if [ "$#" -ne 1 ]; then
	echo "Usage: $0 <executable file>"
	exit 1
fi

if [ ! -d "$directory" ]; then
	echo -e "\033[31mDirectory does not exist: $directory\033[0m"
	exit 1
fi

for file in "$directory"/*
do
	echo "Testing file: $file"
	python3 "$file" || echo -e "\033[31mError occurred with file: $file\033[0m\n"
done