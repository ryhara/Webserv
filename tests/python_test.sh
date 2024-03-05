#!/bin/bash

directory=$1
log_file="./uploads/log_$(date +"%Y%m%d_%H%M%S").log"

if [ "$#" -ne 1 ]; then
	echo "Usage: $0 <executable file>"
	exit 1
fi

if [ ! -d "$directory" ]; then
	echo -e "\033[31mDirectory does not exist: $directory\033[0m"
	exit 1
fi

for file in "$directory"*
do
	echo "Testing file: $file" >> "$log_file"
	echo "" >> "$log_file"
	python3 "$file" >> "$log_file" 2>&1 || echo -e "\033[31mError: $file\033[0m"
done