import requests

url = 'http://localhost:4242'
headers = {'Transfer-Encoding': 'chunked'}

def generate_data():
    # ここでチャンクデータを生成します
    yield 'hello'
    yield 'world'
    yield 'abcdefghijklmnopqrstuvwxyz'
    yield '1234567890'

response = requests.post(url, headers=headers, data=generate_data())
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

