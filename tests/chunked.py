import requests

url = 'http://localhost:4242'
headers = {'Transfer-Encoding': 'chunked'}

def generate_data():
    # ここでチャンクデータを生成します
    yield 'hello'
    yield 'world'

response = requests.post(url, headers=headers, data=generate_data())
print(response.text)

