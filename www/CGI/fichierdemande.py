#!/usr/bin/env python3

import cgi
import cgitb
import sys

cgitb.enable()  # Pour le débogage

print("Content-type: text/html\n")

# Récupère le premier argument de la ligne de commande
file_requested = sys.argv[1] if len(sys.argv) > 1 else "Aucun fichier demandé"

print(f"<html><body>")
print(f"<h1>CGI Script en Python</h1>")
print(f"<p>Fichier demandé : {file_requested}</p>")
print(f"</body></html>")
