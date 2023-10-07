
#**********a faire avant de lancer le script
#pip install requests
#pip install termcolor
#*******************************************

from termcolor import colored
import requests
import json

BASE_URL = "http://localhost:8001"

# Test GET
def test_get(endpoint):
	response = requests.get(f"{BASE_URL}{endpoint}")
	print(colored(f"GET {endpoint}:", 'cyan'), colored(f"{response.status_code}, {response.text}", 'green' if response.status_code == 200 else 'red'))

def test_get_with_params(endpoint, params):
	response = requests.get(f"{BASE_URL}{endpoint}", params=params)
	print(colored(f"GET {endpoint}:", 'cyan'), colored(f"{response.status_code}, {response.text}", 'green' if response.status_code == 200 else 'red'))

# Test POST
#def test_post(endpoint, data):
#	headers = {'Content-type': 'application/python'}
#	response = requests.post(f"{BASE_URL}{endpoint}", python=data, headers=headers)
#	print(colored(f"POST {endpoint}:", 'cyan'), colored(f"{response.status_code}, {response.text}", 'green' if response.status_code == 200 else 'red'))

#def test_post(endpoint, data):
#	headers = {'Content-type': 'application/json'}
#	response = requests.post(f"{BASE_URL}{endpoint}", json=data, headers=headers)
#	print(colored(f"POST {endpoint}:", 'cyan'), colored(f"{response.status_code}, {response.text}", 'green' if response.status_code == 200 else 'red'))

# Test DELETE
def test_delete(endpoint):
	response = requests.delete(f"{BASE_URL}{endpoint}")
	print(colored(f"DELETE {endpoint}:", 'cyan'), colored(f"{response.status_code}, {response.text}", 'green' if response.status_code == 200 else 'red'))

if __name__ == "__main__":
	# Test d'une requête GET sur l'endpoint "/"
	test_get("/")
	test_get("/")

	# Test d'une requête GET sur l'endpoint "/data"
#	test_get_with_params("/nyah", {"key": "value"})
	#test_get_with_params("/", {"key": "value"})

	# Test d'une requête POST sur l'endpoint "/data" avec un objet JSON comme donnée
#	test_post("/data", {"key": "value"})

	# Test d'une requête DELETE sur l'endpoint "/data"
	test_delete("/nyah")
