import requests

url = 'http://localhost:4242'

# POST
data = {'key': '1'}
headers = {'Transfer-Encoding': 'chunked', 'Content-Length': '5'}
response = requests.post(url, data=data, headers=headers)

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