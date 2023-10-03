________________________________________________________________________________________________________________________
# CONF

## parser
- [?] upload_folder directive
- [ ] ip : find_last_of ':' !!! not first of (vu qu'adresse ipv6 en contient plein)
- [ ] error if alias AND root in location

________________________________________________________________________________________________________________________
# CORE

- [ ] errors : lesquelles et comment les traiter (classe d'erreur? lesquelles terminent le programme?)

## Epoll
- [ ] epoll_create : cb? flags?

## Engine
- [ ] find a way to keep track of status codes to handle response's first line
- [ ] deep dive content negociation

- [ ] retour recv 0 : ?
- [ ] timeout
- [ ] check max body size
- [ ] connection header : close after ended or not
- [ ] quid en cas de ports identiques pour 2 servers

________________________________________________________________________________________________________________________
# RESPONSE

- [ ] classes for body and headers
- [ ] mimetypes dans l'autre sens 
> Le fichier mime.types contient des mappings entre les extensions de fichier et leurs types MIME associés. Un type MIME est une chaîne qui indique le type de données que sont certains contenus. Par exemple, text/html est le type MIME pour les documents HTML et image/jpeg est le type MIME pour les images JPEG.

> Lorsqu'un serveur web (comme Nginx) sert un fichier, il inclut un en-tête Content-Type dans la réponse HTTP pour indiquer au client (généralement un navigateur web) comment interpréter les données reçues. Le serveur utilise le fichier mime.types pour déterminer quel type MIME envoyer en fonction de l'extension du fichier.

> Si Nginx sert un fichier example.html, il regardera dans mime.types, verra que les fichiers .html doivent être servis avec le type MIME text/html, et inclura l'en-tête Content-Type: text/html dans la réponse HTTP.

> Pour résumer, le fichier mime.types aide le serveur à déterminer comment informer le client du type de contenu qu'il est en train de recevoir.
________________________________________________________________________________________________________________________
# WHEN EVERYTHING FINISHED

- [ ] find remaining "//TODO"s

## Tests
- [ ] content-length > max body size