#!/bin/bash

log_file="./uploads/log_curl_$(date +"%Y%m%d_%H%M%S")"

if [ -z $1 ]; then
	echo "Usage: $0 <port>"
	exit 1
fi

if [ -z $2 ]; then
	option="-v"
else
	option=$2
fi

port=":$1"
resolve_port=":$1"

if [ $1 -eq 80 ]; then
	port=""
fi

command_list=(
	"curl $option http://localhost$port"
	"curl $option http://localhost$port/hello.html"
	"curl $option http://localhost$port/dir/"
	"curl $option http://localhost$port/dir/subdir/"
	"curl $option http://localhost$port/directory/"
	"curl $option http://localhost$port/autoindex/"
	"curl $option http://localhost$port/redirect/"
	"curl $option http://localhost$port/cgi/test.sh"
	"curl $option -X POST http://localhost$port/cgi/test.sh"
	"curl $option http://localhost$port/cgi/subdir/test.py"
	"curl $option -X POST http://localhost$port/cgi/subdir/test.py"
	"curl $option http://localhost$port/cgi/loop.sh"
	"curl $option -X POST http://localhost$port/cgi/loop.py"
	"curl $option http://localhost$port/cgi/test.rb"
	"curl $option http://localhost$port/cgi/calc.py?value1=2&operator=add&value2=5"
	# "curl $option -X POST http://localhost$port/cgi/calc.py -d value1=2&operator=add&value2=5"
	"curl $option http://localhost$port/test.sh"
	"curl $option --resolve webserv$resolve_port:127.0.0.1 http://webserv$port"
	"curl $option --resolve webserv2$resolve_port:127.0.0.1 http://webserv2$port"
	# "curl $option -X POST -H 'Content-Type: plain/text' --data 'BODY IS HERE write something shorter or longer than body limit' http://localhost$port"
	"curl $option -X DELETE http://localhost$port/delete.html"
	"curl $option -X PUT http://localhost$port"
	# "curl $option -X POST -H 'Transfer-Encoding: chunked' -d 'hello' http://localhost$port"
)


for command in "${command_list[@]}"; do
	echo "==============================================================================" >> "$log_file"
	echo "Running: $command" >> "$log_file"
	echo "" >> "$log_file"
	$command >> "$log_file" 2>&1 || echo -e "\033[31mError: $command\033[0m"
	echo "" >> "$log_file"
	sleep 1
done