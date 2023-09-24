# CORE

## epoll
- [ ] epoll_create : cb? flags?


## readFromClient 
- [ ] retour recv 0 : ?
- [ ] timeout

## erros
- [ ] lesquelles et comment les traiter (classe d'erreur?)

# server
- [ ] keep track of server fd

- [ ] find a way to keep request and response context in regards to clientFD
- [ ] find a way to keep track of status codes to handle response's first line
- [ ] deep dive content negociation


# CONF

## parser
- [?] upload_folder directive
- [x] display of Location within Server
- [x] path of location space en trop
- [x] add alias to location
- [x] pb of booleans
	- [x] autoindex
	- [x] cgi


- [ ] default values in Location constructor
- [ ] setup routes in a server to differents directories
(cf routes_and_locations.md)

une fois que tout est ok
- [ ] error if absence of certain directive / context ? (server {}, listen, ..?)
- [ ] fill with default if empty
