import requests

# GET
url = 'http://localhost:4242/redirect/'
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