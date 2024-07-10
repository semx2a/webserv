# Webserv

[![en](https://img.shields.io/badge/lang-en-pink.svg)](https://github.com/semx2a/webserv/blob/main/README.md)
[![fr](https://img.shields.io/badge/lang-fr-purple.svg)](https://github.com/semx2a/webserv/blob/main/README.fr.md)

## Introduction

Le projet Webserv nous amène à écrire votre propre serveur HTTP en C++ 98 inspiré de NGINX, nous offrant ainsi l'occasion de comprendre les mécanismes internes d'un serveur web et de nous familiariser avec les requêtes et les réponses HTTP.

## Cahier des Charges

- **Langage**: C++ 98 et C.
- **Fonctionnalités** : Le serveur doit :
  - être capable de lire un fichier de configuration et de s'adapter en conséquence.
  - pouvoir ecouter sur plusieurs ports.
  - être non-bloquant et utiliser `poll()` (ou équivalent) pour les opérations d'entrée/sortie.
  - supporter les méthodes GET, POST, et DELETE.
  - être capable de servir un site web statique.
  - gérer le téléversement de fichiers par les clients
  - supporter les CGI pour certaines extensions de fichiers.
  - être robuste et ne jamais cesser de fonctionner inopinément.
  - être conforme à HTTP 1.1, en utilisant NGINX comme référence pour les comportements de réponse et les en-têtes.
- Aucune bibliothèque externe (ni l'ensemble Boost) n'est autorisée.
- Interdiction d'utiliser `fork` pour autre chose que les CGI.

## Utilisation

```shell
git clone git@github.com:semx2a/webserv.git
cd webserv/server
make
./webserv chemin/vers/fichier_de_conf.conf
'''

> Un fichier de configuration de démonstration se trouve dans `neoserv/conf/testwebsite.conf`. Vous pouvez le modifier pour changer le comportement du serveur, les ports d'écoute, etc.

Pour accéder au site, entrez simplement la ligne ci-dessous dans la bare de recherche de votre navigateur web:

```web
http://localhost:8080
```

> Le port 8080 est le port configuré par défaut dans le fichier de configuration fourni. Si ce paramètre est modifié, le port devra être modifié dans le navigateur afin de pouvoir accéder au site

## Apprentissages Liés au Projet

- Compréhension approfondie du protocole HTTP et de ses méthodes.
- Maîtrise de la programmation réseau en C et C++, en particulier la gestion des connexions non-bloquantes et le multiplexage des I/O. Nous avons choisi `epoll` comme multiplexer pour les raisons suivantes:
  - Meilleure Performance: `epoll` fonctionne sur un modèle basé sur les événements, où le kernel informe l'application des descripteurs de fichiers prêts pour l'opération d'entrée/sortie. Cela évite des appels système inutiles (contrairement à `select` ou `poll` qui réitèrent sur l'ensemble des fichiers) et réduit la complexité de O(n) à O(1) dans de nombreux scénarios.
  - Flexibilité et Contrôle: `epoll` offre un contrôle précis sur les types d'événements surveillés pour chaque descripteur de fichier, et ce sans avoir à le retirer et à le réajouter, ce qui n'est pas possible avec `select` ou `poll`.
- Gestion d'un projet complexe en C++, en respectant les normes de codage et en produisant un code propre et bien organisé.
- Capacité à créer et à lire des fichiers de configuration pour personnaliser le comportement du serveur.
- Compréhension de la manière dont les serveurs web traitent les requêtes, servent les contenus statiques, gèrent les téléversements de fichiers et exécutent des CGI.
- Développement de compétences en débogage et en test, pour s'assurer que le serveur est fiable et performant même sous charge.

## Exploration de l'Orienté Objet avec C++

### Liste des concepts utilisés

- Classes
  - Encapsulation
    - Visibilité
    - Attributs et fonctions membre
      - Constantes
    - Accesseurs
  - Constructeurs / Destructeurs
    - Forme canonique de Coplien
    - Liste d'initialisation
  - Pointeurs sur les membres
  - Héritage
    - Classes abstraites  
- Surcharge d'opérateur
- Templates
- Containers
  - Vectors
  - Maps
- Polymorphisme (Surcharge de fonction)
  - Sous-typage
- Compilation conditionnelle
- Exceptions
- Casting

### Exemples

#### Compilation conditionnelle

```cpp
#ifdef LOGS
    utl::log(socket, "Closed connexion");
#endif
```

#### Classes; Héritage; Encapsulation; Polymorphisme; Forme canonique; Constantes

```cpp
#ifndef HEADERS_HPP
# define HEADERS_HPP

# include "ARespComponent.hpp"
# include "ResponseContext.hpp"
# include "MimeTypes.hpp"

class Headers : public ARespComponent {

  public:
      Headers(size_t);
      Headers(std::string const&, size_t, std::string const&, ResponseContext const&, std::string const&);
      Headers(Headers const&);
      ~Headers();
  
      Headers &   operator=(Headers const&);

      size_t      contentLength(void) const;
      void        setContentLength(size_t);

      void        build(void);
      void        build(std::string const&, size_t, std::string const&, ResponseContext const&, std::string const&);

  private:
      Headers();
      size_t      _contentLength;
      MimeTypes   _mimeTypes;

      std::string _findExtension(std::string const& path);
};

#endif
```
