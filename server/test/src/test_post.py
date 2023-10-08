
#**********a faire avant de lancer le script
#pip install requests
#pip install termcolor
#*******************************************

from termcolor import colored
import requests
import json

BASE_URL = "http://localhost:8001"

# test POST sendfile
def send_file(endpoint, file_path):
    headers = {'Content-type': 'multipart/form-data'}
    
    with open(file_path, 'rb') as f:
        files = {'file': f}
        response = requests.post(f"{BASE_URL}{endpoint}", files=files, headers=headers)
        
        print(f"POST {endpoint}: {response.status_code}, {response.text}")

## Test POST
#def test_post(endpoint, data):
#	headers = {'Content-type': 'application/python'}
#	response = requests.post(f"{BASE_URL}{endpoint}", data=data, headers=headers)
#	print(colored(f"POST {endpoint}:", 'cyan'), colored(f"{response.status_code}, {response.text}", 'green' if response.status_code == 200 else 'red'))

if __name__ == "__main__":

    send_file("/data", "posttest.txt")
#	# Test d'une requête POST sur l'endpoint "/data" avec un objet JSON comme donnée
#	test_post("/data", {"key": "value"})

