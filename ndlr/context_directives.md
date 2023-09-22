
# Contexts

## main context

The most general context is the “main” or “global” context. 

Any directive that exists entirely outside of these blocks belongs to the “main” context. Keep in mind that if your Nginx configuration is set up in a modular fashion – i.e., with configuration options in multiple files – some files will contain instructions that appear to exist outside of a bracketed context, but will be included within a context when the configuration is loaded together.

The main context represents the broadest environment for Nginx configuration. It is used to configure details that affect the entire application. While the directives in this section affect the lower contexts, many of these cannot be overridden in lower levels.

Some common details that are configured in the main context are :
- the system user and group to run the worker processes as
- the number of workers
- the file to save the main Nginx process’s ID
- the **default error file** for the entire application can be set at this level (this can be overridden in more specific contexts).

```nginx
error_page fdsflkds?????
```

## server {}

**Syntax**:
```nginx
server { ... }
```

**Default**: —

**Context**: http

Sets configuration for a virtual server. 

There is no clear separation between IP-based (based on the IP address) and name-based (based on the “Host” request header field) virtual servers. 

Instead, the listen directives describe all addresses and ports that should accept connections for the server, and the server_name directive lists all server names. 

Example configurations are provided in the [How nginx processes a request](https://nginx.org/en/docs/http/request_processing.html) document.

## location {}

**Syntax**:
```nginx
location [ = | ~ | ~* | ^~ ] uri { ... }
location @name { ... }
```
**Default**: —

**Context**: server, location

Sets configuration depending on a request URI.

location blocks can be nested, with some exceptions mentioned below.

Also, using the “=” modifier it is possible to define an exact match of URI and location. If an exact match is found, the search terminates. For example, if a “/” request happens frequently, defining “location = /” will speed up the processing of these requests, as search terminates right after the first comparison. Such a location cannot obviously contain nested locations.


```nginx
location = / {
    [ configuration A ]
}

location / {
    [ configuration B ]
}

location /documents/ {
    [ configuration C ]
}

location ^~ /images/ {
    [ configuration D ]
}

location ~* \.(gif|jpg|jpeg)$ {
    [ configuration E ]
}
```

- the “/” request will match configuration A
- the “/index.html” request will match configuration B
- the “/documents/document.html” request will match configuration C
- the “/images/1.gif” request will match configuration D
- the “/documents/1.jpg” request will match configuration E

The “@” prefix defines a named location. Such a location is not used for a regular request processing, but instead used for **request redirection**. They cannot be nested, and cannot contain nested locations.

```nginx
location @name {
	[...]
}
```

# Directives

## alias

**Syntax**:	
```nginx
alias path;
```

**Default**: —

**Context**: location

Defines a replacement for the specified location. For example, with the following configuration

```nginx
location /i/ {
    alias /data/w3/images/;
}
```
on request of “/i/top.gif”, the file /data/w3/images/top.gif will be sent.

The path value can contain variables, except $document_root and $realpath_root.


## client_max_body_size

**Syntax**:	
```nginx
client_max_body_size size;
```

**Default**: 
```nginx
client_max_body_size 1m;
```

**Context**: http, server, location

- Sets the maximum allowed size of the client request body. 
- If the size in a request exceeds the configured value, the 413 (Request Entity Too Large) error is returned to the client. Please be aware that browsers cannot correctly display this error. 
- Setting size to 0 disables checking of client request body size. 


## error_page

**Syntax**:
```nginx
error_page code ... [=[response]] uri;`
```

**Default**: —

**Context**: http, server, location, if in location

- Defines the URI that will be shown for the specified errors. A uri value can contain variables.

*Example*:

```nginx
error_page 404             /404.html;
error_page 500 502 503 504 /50x.html;
```

This causes an internal redirect to the specified uri with the client request method changed to “GET” (for all methods other than “GET” and “HEAD”)

Furthermore, it is possible to change the response code to another using the “=response” syntax, for example:

```nginx
error_page 404 =200 /empty.gif;
```
If an error response is processed by a proxied server or a FastCGI/uwsgi/SCGI/gRPC server, and the server may return different response codes (e.g., 200, 302, 401 or 404), it is possible to respond with the code it returns:

```nginx
error_page 404 = /404.php;
```
If there is no need to change URI and method during internal redirection it is possible to pass error processing into a named location:

```nginx
location / {
    error_page 404 = @fallback;
}

location @fallback {
    proxy_pass http://backend;
}
```

If uri processing leads to an error, the status code of the last occurred error is returned to the client.
It is also possible to use URL redirects for error processing:

```nginx
error_page 403      http://example.com/forbidden.html;
error_page 404 =301 http://example.com/notfound.html;
```
In this case, by default, the response code 302 is returned to the client. It can only be changed to one of the redirect status codes (301, 302, 303, 307, and 308).

The code 307 was not treated as a redirect until versions 1.1.16 and 1.0.13.
The code 308 was not treated as a redirect until version 1.13.0.
These directives are inherited from the previous configuration level if and only if there are no error_page directives defined on the current level.


## authorized_methods

**Syntax**: 
```nginx
authorized_methods method ... { ... }
```

**Default**:	—

**Context**: location

Limits allowed HTTP methods inside a location. 

Allowing the GET method makes the HEAD method also allowed 

```nginx
authorized_methods GET {
    allow 192.168.1.0/32;
    deny  all;
}
```

## listen

**Syntax**:	

```nginx
listen address[:port] [default_server] [ssl] [http2 | quic] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];

listen port [default_server] [ssl] [http2 | quic] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];

`listen unix:path [default_server] [ssl] [http2 | quic] [proxy_protocol] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
```

**Default**: 
```nginx
listen *:80 | *:8000;
```

**Context**: server

Sets the address and port for IP, or the path for a UNIX-domain socket on which the server will accept requests. Both address and port, or only address or only port can be specified. An address may also be a hostname, for example:

```nginx
listen 127.0.0.1:8000;
listen 127.0.0.1;
listen 8000;
listen *:8000;
listen localhost:8000;
```

IPv6 addresses (0.7.36) are specified in square brackets:

```nginx
listen [::]:8000;
listen [::1];
```
UNIX-domain sockets (0.8.21) are specified with the “unix:” prefix:

```nginx
listen unix:/var/run/nginx.sock;
```

**Default**:
- If only address is given, the port 80 is used.
- If the directive is not present then either *:80 is used if nginx runs with the superuser privileges, or *:8000 otherwise.


## server_name

**Syntax**:
```nginx
server_name name ...;
```

**Default:**
```nginx
server_name "";
```

**Context**: server

Sets names of a virtual server, for example:

```nginx
server {
    server_name example.com www.example.com;
}
```
The first name becomes the primary server name.

Server names can include an asterisk (“*”) replacing the first or last part of a name:

```nginx
server {
    server_name example.com *.example.com www.example.*;
}
```

Such names are called wildcard names.

The first two of the names mentioned above can be combined in one:

```nginx
server {
    server_name .example.com;
}
```

It is also possible to specify an empty server name (0.7.11):

```nginx
server {
    server_name www.example.com "";
}
```

It allows this server to process requests without the “Host” header field — instead of the default server — for the given address:port pair. This is the default setting.

Before 0.8.48, the machine’s hostname was used by default.
During searching for a virtual server by name, if the name matches more than one of the specified variants, (e.g. both a wildcard name and regular expression match), the first matching variant will be chosen, in the following order of priority:

1. the exact name
2. the longest wildcard name starting with an asterisk, e.g. “*.example.com”
3. the longest wildcard name ending with an asterisk, e.g. “mail.*”
4. the first matching regular expression (in order of appearance in the configuration file)

Detailed description of server names is provided in a separate [Server names](https://nginx.org/en/docs/http/server_names.html) document.


## root

**Syntax**:	
```nginx
root path;
```

**Default**:	
```nginx
root html;
```

**Context**: http, server, location, if in location

Sets the root directory for requests. For example, with the following configuration

```nginx
location /i/ {
    root /data/w3;
}
```

The /data/w3/i/top.gif file will be sent in response to the “/i/top.gif” request.

The path value can contain variables, except $document_root and $realpath_root.

A path to the file is constructed by merely adding a URI to the value of the root directive. If a URI has to be modified, the alias directive should be used.