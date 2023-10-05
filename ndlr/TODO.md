________________________________________________________________________________________________________________________
# CONF

## parser
- [?] upload_folder directive

________________________________________________________________________________________________________________________
# CORE

- [ ] errors : lesquelles et comment les traiter (classe d'erreur? lesquelles terminent le programme?)

## Epoll
- [ ] epoll_create : cb? flags?

## Engine
- [x] find a way to keep track of status codes to handle response's first line
- [ ] deep dive content negociation

- [ ] retour recv 0 : ?
- [ ] timeout
- [x] check max body size
- [x] connection header : close after ended or not
- [ ] quid en cas de ports identiques pour 2 servers

________________________________________________________________________________________________________________________
# RESPONSE

- [x] classes for body and headers
- [x] mimetypes dans l'autre sens 
- [ ] POST

________________________________________________________________________________________________________________________
# WHEN EVERYTHING FINISHED

- [ ] find remaining "//TODO"s

## Tests
- [ ] content-length > max body size