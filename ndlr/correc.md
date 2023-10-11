
### Global

- [ ] **siege**
- [x] explain basics of HTTP server
- [x] explain why epoll 
- [x] explain how epoll is working
- [x] explain how managed the server accept and the client read/write
- [x] check for read and write at the same time in main loop
- [x] only one read or one write per client per epoll. Show the code that goes from the epoll to the read and write of a client
- [x] search for all read/recv/write/send on a socket and check that if an error returned, the client is removed
- [x] search for all read/recv/write/send and check if the returned value is well checked (checking only -1 or 0 is not good you should check both)
- [x] if a check or errno is done after read/recv/write/send -> 0


- [ ] no wrong HTTP response status code (checked during all evaluation)

### Config

- [x] setup multiple servers with different ports
- [x] setup multiple servers with different hostname 
  		> curl --resolve example.com:80:127.0.0.1 http://example.com
- [x] setup default error page
		> try to change the error 404
- [ ] limit the client body
		> curl -X POST -H "Content-Type: plain/text" --data "BODY IS HERE write stg shorter or longer than body limit"
- [ ] setup routes in a server to different directory
- [ ] setup a default file to search for if you ask for a directory
- [ ] setup a list of method accepted for a certain route 
		> ex: try to delete stg with and without permission

### Basic checks

Using telnet, curl, prepared files demonstrates that the following features work properly:
- [ ] GET requests 
- [ ] POST requests
- [ ] DELETE requests
- [ ] UNKNOWN requests -> should not produce any crash
- [ ] For every test the status code must be good
- [ ] upload some file to the server and get it back

### Check with a browser

- [ ] Use the reference browser of the team, open the network part of it and try to connect to the server with it
- [ ] Look at the request header and response header
- [ ] It should be compatible to serve a fully static website
- [ ] Try a wrong URL on the server
- [ ] Try to list a directory
- [ ] Try a redirected URL
- [ ] Try things

### Port issues

- [ ] In the configuration file, setup multiple ports and use different websites, use the browser to check that the configuration is working as expected, and show the right website
- [ ] In the configuration try to setup the same port multiple times. It should not work
- [ ] Launch multiple servers at the same time with different config but with common ports. Is it working? If it is working, ask why the server should work if one of the configurations isn't working

### Siege and stress test

- [ ] Use Siege to run some stress tests
- [ ] Availability should be above 99.5% for a simple get or an empy page with a siege -b on that page
- [ ] Check if there is no memory leak (monitor the process memory usage it should no go up indefinitely)
- [ ] Check if there is no hanging connection
- [ ] You should be able to use siege indefinitely without restarting the server (look at siege -b)

### Bonus 

- [ ] Cookies and session : There's a working session and cookies system on the webserver
- [ ] CGI : There's more than one CGI system




