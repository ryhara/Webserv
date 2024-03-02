#!/usr/bin/python3

import os

query_string = os.environ['QUERY_STRING']

query_list = query_string.split('&')
a = query_list[0].split('=')[1]
op = query_list[1].split('=')[1]
b = query_list[2].split('=')[1]


selected_plus = ''
selected_sub = ''
selected_mul = ''
selected_div = ''

if op == '+':
    result = int(a) + int(b)
    selected_plus = 'selected'
elif op == '-':
    result = int(a) - int(b)
    selected_sub = 'selected'
elif op == '*':
    result = int(a) * int(b)
    selected_mul = 'selected'
elif op == '/':
    result = int(a) / int(b)
    selected_div = 'selected'

print('<!DOCTYPE html><html><head><meta charset="utf-8"><title>Webserv</title><link rel="icon" href="data:,"></head>')
print('<body><h1>Welcome to Webserv!</h1><p>It works! from /www/cgi.html</p><div>')
print(f'<div><h2>{a} {op} {b} = {result}</h2></div></body></html>')