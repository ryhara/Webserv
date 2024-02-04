#!/bin/sh


./webserv &

server_pid=$!

for i in {0..4}
do
	sleep 1
	curl -v http://localhost:4242/index.html >> result &
done

cat result
lsof -i:4242

sleep 1
kill $server_pid
rm result
