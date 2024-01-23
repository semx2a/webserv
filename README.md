# Projet: Webserv

## Introduction
Le projet Webserv est conçu pour nous plonger dans les détails du protocole HTTP, le protocole le plus utilisé sur Internet. Ce projet nous amène à écrire votre propre serveur HTTP en C++ 98, nous offrant ainsi l'occasion de comprendre les mécanismes internes d'un serveur web et de se familiariser avec les requêtes et les réponses HTTP.

## Contraintes Techniques
- Le serveur doit être écrit en C++ 98.
- Aucune bibliothèque externe ou ensemble Boost n'est autorisé.
- Le serveur doit être non-bloquant et utiliser `poll()` (ou équivalent) pour les opérations d'entrée/sortie.
- Le serveur doit être capable de lire un fichier de configuration et de s'adapter en conséquence.
- Le serveur doit supporter les méthodes HTTP GET, POST, et DELETE.
- Le serveur doit être capable de servir un site web statique, de gérer le téléversement de fichiers par les clients, et de supporter les CGI pour certaines extensions de fichiers.
- Le serveur doit être robuste et ne jamais cesser de fonctionner inopinément.
- Le serveur doit être conforme à HTTP 1.1, en utilisant NGINX comme référence pour les comportements de réponse et les en-têtes.

## Apprentissages Liés au Projet
- Compréhension approfondie du protocole HTTP et de ses méthodes.
- Maîtrise de la programmation réseau en C et C++, en particulier la gestion des connexions non-bloquantes et le multiplexage des I/O. Nous avons choisi `epoll` comme multiplexer pour les raisons suivantes:

	- Meilleure Performance: `epoll` fonctionne sur un modèle basé sur les événements, où le kernel informe l'application des descripteurs de fichiers prêts pour l'opération d'entrée/sortie. Cela évite des appels système inutiles (contrairement à `select` ou `poll` qui réitèrent sur l'ensemble des fichiers) et réduit la complexité de O(n) à O(1) dans de nombreux scénarios.

	- Flexibilité et Contrôle: `epoll` offre un contrôle précis sur les types d'événements surveillés pour chaque descripteur de fichier, et ce sans avoir à le retirer et à le réajouter, ce qui n'est pas possible avec `select` ou `poll`.

- Gestion d'un projet complexe en C++, en respectant les normes de codage et en produisant un code propre et bien organisé.
- Capacité à créer et à lire des fichiers de configuration pour personnaliser le comportement du serveur.
- Compréhension de la manière dont les serveurs web traitent les requêtes, servent les contenus statiques, gèrent les téléversements de fichiers et exécutent des CGI.
- Développement de compétences en débogage et en test, pour s'assurer que le serveur est fiable et performant même sous charge.
