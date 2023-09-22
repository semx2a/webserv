### server conf : `server {}`

- URI : Uniform Resource Identifier - **Un URI peut être de type « locator » ou « name » ou les deux.**
  - URL : Uniform Resource Locator (ex: http://www.wikipedia.org/)
  - URN : Uniform Resource Name (ex: urn:isbn:0-395-36341-1)

	URI = {URL | URN }

	![URI Diagram](https://commons.wikimedia.org/wiki/File:URI_Venn_Diagram.png?uselang=fr)

```nginx
	location / {
		try_files $uri $uri/ =404;
	}
```

- `listen` : ports -> vector 

a) Both address and port, or only address or only port can be specified. An address may also be a hostname, for example:
```nginx
	listen 127.0.0.1:8000;
	listen 127.0.0.1;
	listen 8000;
	listen *:8000;
	listen localhost:8000;
```

b) IPv6 addresses (0.7.36) are specified in square brackets:
```nginx
	listen [::]:8000;
	listen [::1];
```

c) UNIX-domain sockets (0.8.21) are specified with the “unix:” prefix:
```nginx
	listen unix:/var/run/nginx.sock;
```
- `server_name` : plusieurs strings

- `client_max_body_size` : string "10m" MB -> int * 10^7
	- default 1m (100 000 b)
	- error 413 

- `error_page` : string -> path

first server = default

### routes `location {}`

- **accepted HTTP requests** : `authorizedMethods GET POST;`

- **HTTP redirection** : 
  - *Redirection permanente de l'ancienne URL vers la nouvelle*
	```nginx
	location = /ancienne-route {
		return 301 /nouvelle-route;
	}
	```

  - *Redirection temporaire*
	```nginx
	location = /temporaire {
		return 302 /autre-route;
	}
	```

- **Directory file** 

  - *root*
	```nginx
	location /path/ {
		root /tmp/www
	}
	```
	-> www.example.com.com/path : ira chercher dans le repertoire /tmp/www/path

  - *alias*
	```nginx
	location /path/ {
	}
		   alias /tmp/www/;
	```
	-> www.example.com.com/path : ira chercher dans le repertoire /tmp/www

- **Directory listing** : 
	```nginx
	location /some-directory/ {
		autoindex on;
	}
	```
	Avec cette configuration, si quelqu'un accède à /some-directory/ et qu'aucun fichier index n'est trouvé, Nginx affichera une liste des fichiers et sous-répertoires.

	*default: off*

	Avec cette configuration, si quelqu'un accède à /some-directory/ et qu'aucun fichier index n'est trouvé, Nginx retournera une erreur 403 Forbidden

- **Default file if request is a directory**
	```nginx
	location / {
		root /var/www/yourwebsite;
		index index.php index.html index.htm;
	}
	```

- **CGI** : POST and GET methods
	```nginx
		location ~ \.php$ {
			include fastcgi_params;
			fastcgi_pass 127.0.0.1:9000;
			fastcgi_index index.php;
			fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;
		}
	```

	- `include fastcgi_params;`: Inclut un fichier de paramètres FastCGI par défaut.
	- `fastcgi_pass 127.0.0.1:9000;`: Passe les requêtes FastCGI à php-fpm écoutant sur 127.0.0.1 port 9000.
	- `fastcgi_index index.php;`: Définit le fichier index pour les répertoires lors de l'utilisation de FastCGI.
	- `fastcgi_param SCRIPT_FILENAME $document_root$fastcgi_script_name;`: Définit le chemin absolu vers le fichier de script à exécuter.

  
- **Root for uploads**
	```nginx
		location /upload {
			alias /var/www/example.com/upload.php;
		}
	```

- **Additional path information to the script**: `PATH_INFO`
	> *"Because you won’t call the CGI directly, use the full path as PATH_INFO"*

	Lors de la configuration de votre serveur, vous devez spécifier comment la variable PATH_INFO doit être remplie. Comme le script CGI n'est pas appelé directement (mais plutôt via le serveur web), vous devez définir la variable PATH_INFO avec le chemin complet que le script devrait utiliser pour ses opérations.


- **Your program should call the CGI with the file requested as first argument**
	 Lorsque le serveur web (ou votre programme personnalisé) exécute un script CGI, il devrait passer le fichier demandé par l'utilisateur en tant que premier argument de la ligne de commande du script CGI.

	Par exemple, supposons que l'utilisateur accède à http://exemple.com/cgi-bin/script.cgi/fichier.txt. Ici, script.cgi est le script CGI que vous souhaitez exécuter, et fichier.txt est le fichier que l'utilisateur demande.

	Si votre serveur est écrit en Python, vous pourriez utiliser le module subprocess pour appeler le script CGI comme ceci :

	```python
	import subprocess

	subprocess.run(["/chemin/vers/script.cgi", "fichier.txt"])
	```

	Dans cet exemple, 
	- `/chemin/vers/script.cgi` est le chemin complet vers le script CGI 
	- `fichier.txt` est passé comme premier argument.
  	
	Le script CGI peut ensuite utiliser cet argument pour effectuer des opérations spécifiques sur le fichier en question.

	Cette approche permet à un seul script CGI de gérer de multiples fichiers ou requêtes, en fonction de l'argument qui lui est passé, ce qui le rend plus flexible et réutilisable.




