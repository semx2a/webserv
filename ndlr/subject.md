# Mandatory part

## General 

Authorized C functions : execve, dup, dup2, pipe, strerror, gai_strerror, errno, dup, dup2, fork, socketpair, htons, htonl, ntohs, ntohl, select, poll, epoll (epoll_create, epoll_ctl, epoll_wait), kqueue (kqueue, kevent), socket, accept, listen, send, recv, chdir bind, connect, getaddrinfo, freeaddrinfo, setsockopt, getsockname, getprotobyname, fcntl, close, read, write, waitpid, kill, signal, access, stat, opendir, readdir and closedir.

- [ ]  Your server must never block and the client can be bounced properly if necessary. The important thing is resilience. Your server should never die.

- [x] You must provide some configuration files and default basic files to test and demonstrate every feature works during evaluation.

- [x] If you’ve got a question about one behavior, you should compare your program behavior with NGINX’s. For example, check how does server_name work.

- [x] We’ve shared with you a small tester. It’s not mandatory to pass it if everything works fine with your browser and tests, but it can help you hunt some bugs.
Do not test with only one program. Write your tests with a more convenient language such as Python or Golang, and so forth. Even in C or C++ if you want to

### Polling
- [x]  It must be non-blocking and use only 1 poll() (or equivalent) for all the I/O operations between the client and the server (listen included). poll() (or equivalent) must check read and write at the same time.

- [x]  You must never do a read or a write operation without going through poll() (or equivalent).

- [x]  Checking the value of errno is strictly forbidden after a read or a write operation.

- [x]  You don’t need to use poll() (or equivalent) before reading your configuration file.
> Because you have to use non-blocking file descriptors, it is possible to use read/recv or write/send functions with no poll() (or equivalent), and your server wouldn’t be blocking. But it would consume more system resources. Thus, if you try to read/recv or write/send in any file descriptor without using poll() (or equivalent), your grade will be 0.

- [x]  You can use every macro and define like FD_SET, FD_CLR, FD_ISSET, FD_ZERO (understanding what and how they do it is very useful).

- [ ]  A request to your server should never hang forever.

- [x]  Your server must be compatible with the web browser of your choice.

- [x]  We will consider that NGINX is HTTP 1.1 compliant and may be used to compare headers and answer behaviors.

- [ ]  Your HTTP response status codes must be accurate.

- [x]  You server must have default error pages if none are provided.

- [x]  You can’t use fork for something else than CGI (like PHP, or Python, and so forth).

- [x]  You must be able to serve a fully static website.

- [ ]  Clients must be able to upload files.

- [x]  You need at least GET, POST, and DELETE methods.

- [ ]  Stress tests your server. It must stay available at all cost.

- [x]  Your server must be able to listen to multiple ports (see Configuration file).


## Configuration file

- [x]  Your program has to take a configuration file as argument, or use a default path.
You can get some inspiration from the ’server’ part of NGINX configuration file.

In the configuration file, you should be able to:

- [x]  Choose the port and host of each ’server’.

- [ ]  Setup the server_names or not.

- [ ]  The first server for a host:port will be the default for this host:port (that means it will answer to all the requests that don’t belong to an other server).

- [x]  Setup default error pages.

- [x]  Limit client body size.

### Location

Setup routes with one or multiple of the following rules/configuration (routes wont be using regexp):

- [x]  Define a list of accepted HTTP methods for the route.

- [x]  Define a HTTP redirection.

- [x]  Define a directory or a file from where the file should be searched (for example, if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet).

- [ ]  Turn on or off directory listing.

- [x]  Set a default file to answer if the request is a directory.

#### CGI

- [x]  Execute CGI based on certain file extension (for example .php).

- [x]  Make it work with POST and GET methods.

- [ ]  Make the route able to accept uploaded files and configure where they should be saved.

- [x]  Because you won’t call the CGI directly, use the full path as PATH_INFO.

- [x]  Just remember that, for chunked request, your server needs to unchunk it, the CGI will expect EOF as end of the body.

- [ ]  Same things for the output of the CGI. If no content_length is returned from the CGI, EOF will mark the end of the returned data.

- [x]  Your program should call the CGI with the file requested as first argument.

- [x]  The CGI should be run in the correct directory for relative path file access.

- [x]  Your server should work with one CGI (php-CGI, Python, and so forth).
