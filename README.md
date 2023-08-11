# WEBSERV
---
## SERVEUR

### EPOLL OU SELECT

Pour un serveur HTTP en C++ sur Linux, mieux d'utiliser `epoll ()` plutôt que `select ()`.
- `select ()` est plus ancienne et utilise une approche basée sur des descripteurs de fichiers pour la gestion des I/O.
- `epoll ()` est une interface plus moderne et efficace pour la gestion des I/O asynchrones.

1. *Évolutivité* : `epoll ()` offre une meilleure évolutivité lors de la gestion d'un grand nombre de connexions simultanées.
    Il utilise une structure de données interne plus performante et permet de surveiller efficacement des milliers de descripteurs de fichiers.

2. *Performances* : `epoll ()` est généralement plus performant que `select ()` en termes d'utilisation des ressources système.
    Il utilise des mécanismes de notification événementielle, ce qui signifie qu'il n'est pas nécessaire de vérifier périodiquement 
    les descripteurs de fichiers pour détecter les événements.

3. *Prise en charge des bords* : `epoll ()` prend en charge la détection des événements de bords (edge-triggered), ce qui signifie 
    qu'il vous informe uniquement lorsque de nouveaux événements se produisent, plutôt que lorsqu'un descripteur de fichier est prêt en 
    lecture ou en écriture.

### ADDRINFO OU SOCKADDR_IN

La structure `addrinfo` et la structure `sockaddr_in` sont toutes deux utilisées pour la manipulation des adresses réseau dans les sockets.
Cependant, elles ont des utilisations légèrement différentes.

1. La structure `addrinfo` 
    Est généralement utilisée pour la résolution d'adresse.
    C'est-à-dire pour convertir un nom d'hôte (comme "www.example.com") en une adresse IP utilisable par une socket. 
    Cette structure est souvent utilisée en conjonction avec les fonctions `getaddrinfo`() et `getnameinfo()`. 
    Elle offre une plus grande flexibilité et une meilleure prise en charge des protocoles IPv4 et IPv6.

2. La structure `sockaddr_in`
    Est spécifiquement conçue pour représenter une adresse IPv4 
    Est souvent utilisée pour spécifier l'adresse IP et le numéro de port d'un serveur ou d'un client. 
    Cette structure est couramment utilisée avec les fonctions `socket()`, `bind()`, `connect()`, etc., pour établir des connexions réseau.

Dans le contexte d'un serveur HTTP, il est généralement préférable d'utiliser la structure `sockaddr_in` pour spécifier l'adresse IP 
et le port sur lequel le serveur écoute les requêtes HTTP entrantes. Cependant, pour la résolution d'adresse initiale, il peut être utile 
d'utiliser la structure `addrinfo` afin de prendre en charge à la fois les adresses IPv4 et IPv6.


### EPOLL_CTL

La fonction `epoll_ctl` est utilisée pour manipuler l'ensemble epoll.
Voici les actions disponibles :

`EPOLL_CTL_ADD`: Cette action indique à epoll d'ajouter le descripteur de fichier spécifié à l'ensemble. Vous pouvez ensuite surveiller 
    les événements d'I/O sur ce descripteur en utilisant d'autres fonctions epoll.

`EPOLL_CTL_MOD`: Cette action est utilisée pour modifier les options d'un descripteur de fichier existant dans l'ensemble epoll. 
    Vous pouvez utiliser cette action pour modifier les événements d'intérêt ou les données associées à un descripteur déjà présent 
    dans l'ensemble epoll.

`EPOLL_CTL_DEL`: Cette action est utilisée pour supprimer un descripteur de fichier de l'ensemble epoll. Lorsque vous n'avez plus besoin 
    de surveiller les événements d'I/O sur un descripteur de fichier, vous pouvez l'enlever de l'ensemble en utilisant cette action.

`EPOLL_CTL_DELALL`: Cette action est spécifique à certains systèmes d'exploitation et permet de supprimer tous les descripteurs de 
    fichiers de l'ensemble epoll. Cela est utile lorsque vous souhaitez libérer complètement l'ensemble epoll et supprimer tous les 
    descripteurs qui y sont associés.


### ADDRINFO

```C
# include <netdb.h>

struct addrinfo {
    int              ai_flags;     // Indicateurs spécifiques pour la résolution d'adresse, tels que AI_PASSIVE pour une utilisation avec un serveur
    int              ai_family;    // Famille d'adresses, telle que AF_INET pour les adresses IPv4 ou AF_INET6 pour les adresses IPv6.
    int              ai_socktype;  // Type de socket, comme SOCK_STREAM pour un socket orienté connexion (TCP) ou SOCK_DGRAM pour un socket datagramme (UDP).
    int              ai_protocol;  // Protocole associé à l'adresse, par exemple IPPROTO_TCP pour TCP ou IPPROTO_UDP pour UDP (0 pour aucun specifique)
    socklen_t        ai_addrlen;   // Longueur de l'adresse en octets
    struct sockaddr *ai_addr;      // Pointeur vers une struct `sockaddr` contenant l'adresse
    char            *ai_canonname; // Nom canonique de l'hote associe a l'adresse

    struct addrinfo *ai_next;      // Pointeur vers l'element suivant dans une liste chainee, permettant de recup plusieurs resultats lors de la resolution de l'adresse
};
```
Quand on utilise la fonction getaddrinfo() pour résoudre un nom d'hôte, on obtient une liste de structures addrinfo contenant les informations
sur les adresses réseau correspondantes, telles que l'adresse IP, le port, etc., nécessaires pour établir une connexion réseau.

La structure addrinfo simplifie la résolution d'adresse en encapsulant toutes les informations nécessaires dans une seule structure,
ce qui facilite la manipulation des adresses réseau dans les programmes en langage C.

```C
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int getaddrinfo(const char *restrict node,
                const char *restrict service,
                const struct addrinfo *restrict hints,
                struct addrinfo **restrict res);

void freeaddrinfo(struct addrinfo *res);

const char *gai_strerror(int errcode);

```

The port number is used by the kernel to match an incoming packet to a certain process’s socket descriptor.

## PARSING