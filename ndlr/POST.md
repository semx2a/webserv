MÉTHODE POST
│
├── 1. Analyse de la requête
│   ├── a. Vérifier la validité de la requête
│   │   ├── i.	Vérifier que la méthode est "POST"
│   │   ├── ii.	L'URI est-il valide?
│   │   ├── iii.Version HTTP correcte (probablement "HTTP/1.1")
│   │   ├── iv.	Présence de l'en-tête "Content-Length" (indispensable pour connaître la taille du corps)
│   │   └── v.	* Vérifier d'autres headers pertinents (comme "Content-Type")
│   ├── b. Extrait de l'URI
│   └── c. Extrait des headers (comme `Host`, `Content-Type`, etc.)
│
├── 2. Traitement en fonction de la configuration
│   ├── a. Recherche de la configuration du serveur correspondant à `host:port`
│   ├── b. Recherche d'une éventuelle redirection définie dans la configuration
│   └── c. Déterminer le chemin physique du fichier ou du répertoire demandé (si applicable)
│
├── 3. Traitement du corps de la requête
│   ├── a. Utiliser l'en-tête "Content-Length" pour lire le nombre correct d'octets du corps
│   ├── b. Si "Content-Type" est "multipart/form-data", analyser le corps pour les fichiers téléchargés
│   ├── c. Si l'URI cible un CGI (par ex. .php), préparer les données pour le CGI
│   └── d. Si l'URI cible un emplacement pour le téléchargement, traiter le téléchargement du fichier
│
├── 4. Préparation de la réponse
│   ├── a. Si le POST était destiné à un CGI
│   │   ├── i.	Exécuter le CGI et capturer sa sortie
│   │   ├── ii.	Définisser le status approprié (200 OK, 500 Internal Server Error, etc.)
│   │   ├── iii.Ajouter les headers nécessaires
│   │   └── iv.	Préparer le corps de la réponse avec la sortie du CGI
│   ├── b. Si le POST était destiné à un téléchargement de fichier
│   │   ├── i. Confirmer que le téléchargement a réussi (201 Created) ou renvoyer une erreur appropriée (par ex. 403 Forbidden)
│   └── c. Si une erreur se produit pendant ce processus, server une page d'erreur par défaut (comme défini dans la configuration)
│
└── 5. Envoi de la réponse
    ├── a. Utiliser poll() ou l'équivalent pour s'assurer que le socket est prêt à envoyer des données
    ├── b. Envoyer la réponse en morceaux (si  nécessaire) sans bloquer
    └── c. Fermer la connexion si nécessaire (par ex. si l'en-tête `Connection: close` est présent)



### 1.a.v. Headers pertinents pour la méthode POST en fonction du sujet:

- **Content-Type**: Indique le type de données dans le corps de la requête. Par exemple, lors de l'envoi d'un formulaire, il peut être "application/x-www-form-urlencoded" ou "multipart/form-data" pour le téléchargement de fichiers.
- **Content-Length**: Essentiel pour connaître la taille du corps de la requête.
- **Host**: Tout comme avec GET, nécessaire pour déterminer le serveur virtuel cible.
- **User-Agent**: Bien que non essentiel, il peut être utile pour déterminer le client effectuant la requête.
- **Authorization**: Pour les éventuelles fonctionnalités d'authentification.
- **Connection**: Indique si la connexion doit rester ouverte ou être fermée après la requête.

### 3.b: Si Content-Type est multipart/form-data, analyser le corps pour les fichiers téléchargés

Si le client envoie des fichiers (par exemple, un téléchargement d'image via un formulaire), la requête POST aura un Content-Type de multipart/form-data, et le corps de la requête contiendra les fichiers téléchargés codés. Analyser ce corps pour extraire ces fichiers.