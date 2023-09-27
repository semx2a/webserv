
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
