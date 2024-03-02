import requests

url = 'http://localhost:4242'

# POST
with open('hoge.dummy', 'rb') as f:
	file_data = f.read()
response = requests.post(url, data=file_data)

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