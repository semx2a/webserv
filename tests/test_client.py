
#**********a faire avant de lancer le script
#pip install requests
#*******************************************

import requests
import json

BASE_URL = "http://localhost:8080"

# Test GET
def test_get(endpoint):
    response = requests.get(f"{BASE_URL}{endpoint}")
    print(f"GET {endpoint}: {response.status_code}, {response.text}")

# Test POST
def test_post(endpoint, data):
    headers = {'Content-type': 'application/json'}
    response = requests.post(f"{BASE_URL}{endpoint}", json=data, headers=headers)
    print(f"POST {endpoint}: {response.status_code}, {response.text}")

# Test DELETE
def test_delete(endpoint):
    response = requests.delete(f"{BASE_URL}{endpoint}")
    print(f"DELETE {endpoint}: {response.status_code}, {response.text}")

if __name__ == "__main__":
    # Test d'une requête GET sur l'endpoint "/"
    test_get("/")

    # Test d'une requête POST sur l'endpoint "/data" avec un objet JSON comme donnée
    test_post("/data", {"key": "value"})

    # Test d'une requête DELETE sur l'endpoint "/data"
    test_delete("/data")
