import requests

url = 'http://localhost:4242'

# # GET
url += "/index.html"
response = requests.get(url)

# POST
# data = {'key1': 'value1'}
# response = requests.post(url, data=data)

# DELETE
# url = 'http://localhost:4242/delete.html'
# response = requests.delete(url)


# request
print("--------------------")
print("URL:", response.request.url)
print("Method:", response.request.method)
print("Headers:")
for key, value in response.request.headers.items():
	print(key, ":", value)
print("Body:", response.request.body)
print("--------------------")
# response
print("Status Code:", response.status_code)
print("Headers:")
for key, value in response.headers.items():
	print(key, ":", value)
print(response.text)