import requests

url_list = [
	'http://localhost:4242',
	'http://localhost:4242/dir/',
	'http://localhost:4242/dir/subdir/test.html',
	'http://localhost:4242/hello.html',
]


for url in url_list:
	# GET
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