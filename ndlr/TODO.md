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

________________________________________________________________________________________________________________________
# WHEN EVERYTHING FINISHED

- [ ] find remaining "//TODO"s

## Tests
- [ ] content-length > max body size