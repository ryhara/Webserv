import requests

url_list = [
	'http://localhost:4242/cgi/test.sh',
	'http://localhost:4242/cgi/subdir/test.py',
	'http://localhost:4242/cgi/test.rb',
	'http://localhost:4242/cgi/loop.py',
	'http://localhost:4242/cgi/calc.py',
]


for index, url in enumerate(url_list):
	# GET
	if index == 4:
		response = requests.get(url, params="value1=1&operator=add&value2=2")
	else:
		response = requests.get(url)
	# request
	print("--------------------------------------------------------------")
	print("URL:", response.request.url)
	print("Method:", response.request.method)
	print("Headers:")
	for key, value in response.request.headers.items():
		print(key, ":", value)
	print("Body:", response.request.body)
	print("--------------------------------------------------------------")
	# response
	print("Status Code:", response.status_code)
	print("Headers:")
	for key, value in response.headers.items():
		print(key, ":", value)
	print(response.text)
	print("==============================================================")