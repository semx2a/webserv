# CORE

## epoll
- [ ] epoll_create : cb? flags?

## errors
- [ ] lesquelles et comment les traiter (classe d'erreur?)

## Engine
- [x] keep track of server fd
- [x] find a way to keep request and response context in regards to clientFD
- [ ] find a way to keep track of status codes to handle response's first line
- [ ] deep dive content negociation

### readFromClient 
- [ ] retour recv 0 : ?
- [ ] timeout
- [ ] check max body size


# CONF

## parser
- [?] upload_folder directive
- [x] display of Location within Server
- [x] path of location space en trop
- [x] add alias to location
- [x] pb of booleans
    - [x] autoindex
    - [x] cgi
- [x] default values in Location constructor
- [ ] setup routes in a server to differents directories
(cf routes_and_locations.md)

une fois que tout est parse
- [x] parcourir les server context et chercher ceux dont 
	- aucune directive listen n'est specifiee -> 0.0.0.0:80
	- aucun server name n'est specifie -> le premier = default. s'arrete la
    - aucune root -> notre path
    - aucun index -> index.html
	- aucune error_page -> nos error pages
	- cgi : ?
	- authorized_methods : GET, POST, DELETE

- [x] fill with default if empty

- [ ] ip : find_last_of ':' !!! not first of (vu qu'adresse ipv6 en contient plein)



# WHEN EVERYTHING FINISHED

- [ ] find remaining "//TODO"s