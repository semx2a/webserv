
#**********a faire avant de lancer le script
#pip install requests
#pip install termcolor
#*******************************************

from termcolor import colored
import requests
import json

BASE_URL = "http://localhost:8001"


# Test POST
def test_post(endpoint, data):
	headers = {'Content-type': 'application/python'}
	response = requests.post(f"{BASE_URL}{endpoint}", data=data, headers=headers)
	print(colored(f"POST {endpoint}:", 'cyan'), colored(f"{response.status_code}, {response.text}", 'green' if response.status_code == 200 else 'red'))

if __name__ == "__main__":

	# Test d'une requête POST sur l'endpoint "/data" avec un objet JSON comme donnée
	test_post("/data", {"key": "value"})

