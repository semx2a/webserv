
### epoll_initialization
epoll_create : cb? flags?


### readFromClient 
- isRequestEnded : check more than \r\n
- retour recv 0 : ?
- timeout

### erros
- lesquelles et comment les traiter (classe d'erreur?)

### server
- keep track of server fd

- [ ] find a way to keep request and response context in regards to clientFD
- [ ] find a way to keep track of status codes to handle response's first line
- [ ] deep dive content negociation