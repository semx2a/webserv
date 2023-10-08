
#**********a faire avant de lancer le script
#pip install requests
#pip install termcolor
#*******************************************

from termcolor import colored
import requests
import json

BASE_URL = "http://localhost:8001"

# Test DELETE
#def test_delete(endpoint):
#	response = requests.delete(f"{BASE_URL}{endpoint}")
#	print(colored(f"DELETE {endpoint}:", 'cyan'), colored(f"{response.status_code}, {response.text}", 'green' if response.status_code == 200 else 'red'))

if __name__ == "__main__":

# Test d'une requête DELETE sur l'endpoint "/data"
#	test_delete("/nyah")