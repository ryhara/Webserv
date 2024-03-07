#!/usr/local/bin/python3

import os

# print(len(os.environ))
# for key in os.environ.keys():
#     print(f'{key}={os.environ[key]}')
query_string = os.environ['QUERY_STRING']

query_list = query_string.split('&')
a = query_list[0].split('=')[1]
op = query_list[1].split('=')[1]
b = query_list[2].split('=')[1]

result = 0

if op == 'add':
    result = int(a) + int(b)
    op = "&plus;"
elif op == 'sub':
    result = int(a) - int(b)
    op = "&minus;"
elif op == 'mul':
    result = int(a) * int(b)
    op = "&times;"
elif op == 'div':
    result = int(a) / int(b)
    op = "&divide;"

print('<!DOCTYPE html><html><head><meta charset="utf-8"><title>Webserv</title><link rel="icon" href="data:,"></head>')
print('<body><h1>Welcome to Webserv!</h1><p>It works! from /www/cgi.html</p><div>')
print(f'<div><h2>{a} {op} {b} = {result}</h2></div></body></html>')