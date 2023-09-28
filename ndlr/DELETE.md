MÉTHODE DELETE
│
├── 1. Analyser la requête
│   ├── a. Vérifier la validité de la requête
│   │   ├── i. S'assurer que la méthode est "DELETE"
│   │   ├── ii. Vérifier que l'URI est valide
│   │   └── iii. Extraire des headers pertinents (comme `Host`)
│   ├── b. Extraire de l'URI
│   └── c. Identifier le serveur et l'emplacement à partir de la configuration
│
├── 2. Traiter en fonction de la configuration
│   ├── a. Chercher la configuration du serveur correspondant à `host:port`
│   ├── b. Chercher une éventuelle redirection définie dans la configuration
│   └── c. Déterminer le chemin physique du fichier ou du répertoire à supprimer
│
├── 3. Supprimer la ressource
│   ├── a. Vérifier si la ressource existe
│   ├── b. Vérifier les permissions pour supprimer la ressource
│   └── c. Supprimer la ressource (fichier ou répertoire)
│
├── 4. Préparer la réponse
│   ├── a. Si la ressource a été supprimée avec succès, préparer une réponse 200 OK ou 204 No Content
│   ├── b. Si la ressource n'existait pas, préparer une réponse 404 Not Found
│   ├── c. Si une permission est refusée ou une autre erreur se produit, préparer une réponse appropriée (par exemple, 403 Forbidden ou 500 Internal Server Error)
│   └── d. Si une erreur se produit pendant ce processus, servir une page d'erreur par défaut (comme défini dans la configuration)
│
└── 5. Envoyer la réponse
    ├── a. Utiliser poll() ou l'équivalent pour s'assurer que le socket est prêt à envoyer des données
    ├── b. Envoyer la réponse sans bloquer
    └── c. Fermer la connexion si nécessaire


### 1.a.iii. Headers pertinents
- **Host**: Comme avec les autres méthodes, le header "Host" est utilisé pour déterminer le serveur virtuel cible, surtout s'il y a plusieurs configurations de serveur pour différents noms d'hôte ou domaines.
- **User-Agent**: Même si ce n'est pas essentiel pour le fonctionnement de la méthode DELETE, cela pourrait être utile à des fins de journalisation ou pour identifier le client effectuant la requête.
- **Authorization**: Si vous implémentez des fonctionnalités d'authentification, ce header serait essentiel pour déterminer si l'utilisateur est autorisé à effectuer l'opération DELETE.
- **Connection**: Ce header vous permettra de savoir si la connexion doit rester ouverte ou être fermée après la requête.

### 3.b. Vérification des permissions:
Pour vérifier les permissions d'un fichier ou d'un répertoire en C ou C++ : `stat()`.

  ```c
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>

  struct stat st;
  if (stat("/path/to/resource", &st) == 0) {
      if (!(st.st_mode & S_IWUSR)) {
          // La ressource n'est pas accessible en écriture pour l'utilisateur actuel
      }
  }
  ```
