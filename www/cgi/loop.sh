#!/bin/bash

loop_count=0

while true ; do
	loop_count=$((loop_count+1))
	echo "loop count: $loop_count"
	sleep 1
	if [ $loop_count -eq 10 ]; then
		break
	fi
done