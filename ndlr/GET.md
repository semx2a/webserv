MÉTHODE GET
│
├── 1. Analyse de la requête
│   ├── a. * Vérifier la validité de la requête (format, headers, version HTTP, etc.)
│   ├── b. Extrait de l'URI 
│   └── c. * Extrait des headers (comme `Host`, `Accept`, etc.)
│
├── 2. Traitement en fonction de la configuration
│   ├── a. Recherche de la configuration du serveur correspondant à `host:port`
│   │   ├── i. Si aucun serveur spécifique n'est trouvé, utiliser le serveur par défaut pour ce `host:port`
│   ├── b. Recherche d'une éventuelle redirection définie dans la configuration
│   └── c. Déterminer le chemin physique du fichier ou du répertoire demandé
│
├── 3. Traitement du chemin
│   ├── a. Si c'est une URI de répertoire
│   │   ├── i. Vérifier si la liste des répertoires est activée
│   │   ├── ii. Vérifier si un fichier par défaut est défini (par exemple index.html)
│   │   └── iii. Si ni i ni ii, retourner une erreur (par exemple 403 Forbidden ou server la page d'erreur par défaut)
│   ├── b. Si c'est une URI de fichier
│   │   ├── i. Vérifier l'existence du fichier
│   │   ├── ii. Vérifier les permissions (le fichier est-il lisible?)
│   │   └── iii. Si c'est un fichier CGI (par ex. .php)
│   │       ├── - Exécuter le CGI avec le fichier demandé comme premier argument
│   │       ├── - Vérifier les headers de sortie du CGI
│   │       └── - Envoie la sortie au client
│   └── c. Si le chemin ne correspond ni à un fichier ni à un répertoire, retourner une erreur (par exemple 404 Not Found ou server la page d'erreur par défaut)
│
├── 4. Préparation de la réponse
│   ├── a. Définir le status approprié (200 OK, 404 Not Found, etc.)
│   ├── b. Ajouter les headers nécessaires (comme `Content-Type`, `Content-Length`, etc.)
│   ├── c. Préparer le corps de la réponse (c'est-à-dire le contenu du fichier ou la sortie du CGI)
│   └── d. Si une erreur se produit pendant ce processus, server une page d'erreur par défaut (comme défini dans la configuration)
│
└── 5. Envoi de la réponse
    ├── a. Utiliser poll() ou l'équivalent pour s'assurer que le socket est prêt à envoyer des données
    ├── b. Envoyer la réponse en morceaux (si nécessaire) sans bloquer
    └── c. Fermer la connexion si nécessaire (par ex. si l'en-tête `Connection: close` est présent)



### 1.a.Pour vérifier la validité de la requête GET, voici une liste d'étapes à considérer:

- **Méthode**: Vérifier que la méthode est "GET".
- **URI**: L'URI ne devrait pas être vide et devrait avoir un format valide (par exemple, ne contient pas de caractères non autorisés).
- **Version HTTP**: Vérifier que la version HTTP est HTTP/1.1 
- **Headers**: La présence de certains headers peut être obligatoire selon la spécification HTTP et les besoins de votre application. Assurer-vous que les headers n'ont pas de caractères non autorisés et sont bien formatés (nom du header suivi de ":" suivi de la valeur).
- **Absence de corps**: Une requête GET valide ne doit pas avoir de corps. Si du contenu est présent après les headers, cela pourrait être une indication d'une mauvaise requête.


### 1.c. Headers utiles pour la méthode GET dans le cadre du sujet:

- **Host**: Ce header est essentiel pour déterminer le serveur virtuel cible lorsqu'on utilise des noms d'hôte basés sur des noms. Il peut également être utilisé pour déterminer le port si spécifié.
- **User-Agent**: Il peut être utile de savoir quel navigateur ou client fait la requête, bien que ce ne soit pas strictement nécessaire pour la fonctionnalité de base.
- **Accept**: Ce header peut dire quels types de médias (MIME) le client accepte, ce qui pourrait être utile pour déterminer le type de contenu à renvoyer.
- **If-Modified-Since**: Utilisé pour le cache. Permet au client de demander une ressource uniquement si elle a été modifiée depuis la date spécifiée.
- **Connection**: Bien que généralement utilisé pour spécifier si la connexion doit rester ouverte ("keep-alive") ou être fermée après la requête ("close"), ce header est important à considérer pour la gestion de la connexion.
- **Authorization**: Si une forme d'authentification est implementee, ce header est utilisé pour transmettre les informations d'authentification.

### 3.c: Le serveur doit pouvoir exécuter des CGI basés sur certaines extensions de fichiers (comme .php) -> doit être capable de gérer des scripts CGI, par exemple, en PHP, Python, etc.

- **Gestion des CGI**: Quand une requête arrive pour une ressource ayant une extension associée à un CGI (par exemple, .php), le serveur doit exécuter ce script plutôt que de simplement renvoyer le fichier. Il faut donc appeler le programme approprié (par exemple, php-cgi pour les scripts PHP) et lui transmettre le script à exécuter.
- **Création des CGI**: Pas besoin de créer des interpréteurs CGI comme PHP ou Python, mais il faut ecrire quelques scripts de test pour s'assurer que le serveur les exécute correctement.
- **Capturer la sortie d'un CGI**: Quand on execute un script CGI, il produira une sortie (qui sera la réponse HTTP à renvoyer au client). Pour capturer cette sortie dans votre programme C++, on utilise généralement des mécanismes comme pipe(), fork(), et dup2().
  - créer un pipe
  - faire un fork
  - dans le processus enfant, rediriger la sortie standard (stdout) vers le pipe, puis exécutez le script CGI
  - dans le processus parent, lire la sortie du script depuis l'autre extrémité du pipe

### 3.a Autoindex
If a request ends with a slash, NGINX treats it as a request for a directory and tries to find an index file in the directory. The index directive defines the index file’s name (the default value is index.html). To continue with the example, if the request URI is /images/some/path/, NGINX delivers the file /www/data/images/some/path/index.html if it exists. If it does not, NGINX returns HTTP code 404 (Not Found) by default. To configure NGINX to return an automatically generated directory listing instead, include the on parameter to the autoindex directive: