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
- [ ] content-type fix
- [ ] CGI : 
      - [ ] sstream that sets headers
      - [ ] expand target for 
- [ ] error page in conf
- [ ] curl localhost:8000 segfault (connection keep-alive)
- [ ] longer path for server context loc
- [ ] pb of GET and POST not considered as in ma
- [ ] pb of :

<html><body><h1>501</h1></body></html>
  ____________________________________________________________
 | October 08 2023 12:45:57                      Client fd: 6 |
 | End of connexion                                           |
  ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾
  ____________________________________________________________
 | October 08 2023 12:45:57                      Client fd: 6 |
 | Buffer received                                            |
  ‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾

Request not ended
^C

________________________________________________________________________________________________________________________
# WHEN EVERYTHING FINISHED

- [ ] find remaining "//TODO"s

## Tests
- [ ] content-length > max body size