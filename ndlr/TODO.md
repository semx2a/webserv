________________________________________________________________________________________________________________________
# CONF

## parser
- [ ] upload_folder directive

________________________________________________________________________________________________________________________
# CORE

- [x] errors : lesquelles et comment les traiter (classe d'erreur? lesquelles terminent le programme?)

## Epoll
- [x] epoll_create : cb? flags?

## Engine
- [x] find a way to keep track of status codes to handle response's first line
- [ ] deep dive content negociation

- [x] retour recv 0 : ?
- [ ] timeout
- [x] check max body size
- [x] connection header : close after ended or not
- [x] quid en cas de ports identiques pour 2 servers

________________________________________________________________________________________________________________________
# RESPONSE

- [x] classes for body and headers
- [x] mimetypes dans l'autre sens 
- [ ] POST
      - [ ] verifier comment un script php change le contenu d'une page html et l'envoie dans la réponse
      - [ ] envoyer le resultat du runCGI au body de la réponse SI et seulement SI le script renvoie un output
- [x] content-type fix
- [ ] CGI : 
      - [ ] sstream that sets headers
      - [ ] expand target for 
- [x] error page in conf
- [x] curl localhost:8000 segfault (connection keep-alive)
- [x] longer path for server context loc
- [ ] pb of GET and POST not considered as in ma

________________________________________________________________________________________________________________________
# WHEN EVERYTHING FINISHED

- [ ] find remaining "//TODO"s
- [ ] still reachable when ctrl-C

## Tests
- [x] content-length > max body size
