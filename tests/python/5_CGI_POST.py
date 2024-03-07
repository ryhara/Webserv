import requests

url_list = [
	'http://localhost:4242/cgi/test.sh',
	'http://localhost:4242/cgi/subdir/test.py',
	'http://localhost:4242/cgi/test.rb',
	'http://localhost:4242/cgi/loop.sh',
	'http://localhost:4242/cgi/calc.py',
	'http://localhost:4242/cgi/calc.py',
]

def generate_data():
	yield 'value1=2'
	yield '&'
	yield 'operator=mul'
	yield '&'
	yield 'value2=3'

for index, url in enumerate(url_list):
	# POST
	if index == 4:
		response = requests.post(url, data="value1=1&operator=add&value2=2")
	elif index == 5:
		response = requests.post(url, data=generate_data())
	else:
		response = requests.post(url)

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