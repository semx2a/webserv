#!/bin/bash

SERVER_URL="http://localhost:8080"

echo "1. GET Request (No body):"
curl -v "$SERVER_URL/path"

echo -e "\n2. POST Request with Content-Length (No chunked encoding):"
curl -v -X POST -d "data_to_send" "$SERVER_URL/path"

echo -e "\n3. POST Request with Chunked Encoding:"
curl -v -X POST -H "Transfer-Encoding: chunked" --data "chunked_data_to_send" "$SERVER_URL/path"

echo -e "\n4. POST Request with Both Content-Length and Chunked Encoding:"
curl -v -X POST -H "Content-Length: 16" -H "Transfer-Encoding: chunked" --data "ambiguous_data" "$SERVER_URL/path"

echo -e "\n5. GET Request with Additional Headers:"
curl -v -H "Custom-Header: custom_value" "$SERVER_URL/path"

echo -e "\n6. Sending a File (for example, an image):"
curl -v -X POST -F "file=@path_to_image.jpg" "$SERVER_URL/upload"

echo -e "\n7. Using Different HTTP Methods (e.g., DELETE):"
curl -v -X DELETE "$SERVER_URL/resource/id"

echo -e "\n8. Sending JSON Data:"
curl -v -X POST -H "Content-Type: application/json" -d '{"key": "value"}' "$SERVER_URL/path"

