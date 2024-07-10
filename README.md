# Webserv

[![en](https://img.shields.io/badge/lang-en-pink.svg)](https://github.com/semx2a/webserv/blob/master/README.md)
[![fr](https://img.shields.io/badge/lang-fr-purple.svg)](https://github.com/semx2a/webserv/blob/master/README.fr.md)

## Introduction

The Webserv project lead us to write our own HTTP server in C++ 98 inspired by NGINX, giving us the opportunity to understand the internal mechanisms of a web server and familiarize ourselves with HTTP requests and responses.

## Specifications

- **Language**: C++ 98 and C.
- **Features**: The server must:
  - Read a configuration file and adapt accordingly.
  - Listen on multiple ports.
  - Be non-blocking and use `poll()` (or equivalent) for I/O operations.
  - Support GET, POST, and DELETE methods.
  - Serve a static website.
  - Handle file uploads from clients.
  - Support CGI for certain file extensions.
  - Be robust and never crash unexpectedly.
  - Comply with HTTP 1.1, using NGINX as a reference for response behaviors and headers.
- No external libraries (including the Boost set) are allowed.
- The use of `fork` is restricted to CGI purposes only.

## Usage

```shell
git clone git@github.com:semx2a/webserv.git
cd webserv/server
make
./webserv path/to/config_file.conf
```

> A demo configuration file is located in `neoserv/conf/testwebsite.conf`. You can modify it to change the server behavior, listening ports, etc.

To access the site, simply enter the following line in your web browser's search bar:

```web
http://localhost:8080
```

> Port 8080 is the default port configured in the provided configuration file. If this parameter is changed, the port must be modified in the browser to access the site.

## Project Learnings

- In-depth understanding of the HTTP protocol and its methods.
- Mastery of network programming in C and C++, especially managing non-blocking connections and I/O multiplexing. We chose `epoll` as the multiplexer for the following reasons:
  - Better Performance: `epoll` operates on an event-driven model, where the kernel informs the application about file descriptors ready for I/O operations. This avoids unnecessary system calls (unlike `select` or `poll` which iterate over the entire set of files) and reduces the complexity from O(n) to O(1) in many scenarios.
  - Flexibility and Control: `epoll` offers precise control over the types of events monitored for each file descriptor without having to remove and re-add it, which is not possible with `select` or `poll`.
- Managing a complex project in C++, adhering to coding standards, and producing clean and well-organized code.
- Ability to create and read configuration files to customize server behavior.
- Understanding how web servers handle requests, serve static content, manage file uploads, and execute CGI.
- Developing debugging and testing skills to ensure the server is reliable and performs well even under load.

## Exploring Object-Oriented Programming with C++

### List of Concepts Used

- Classes
  - Encapsulation
    - Visibility
    - Member attributes and functions
      - Constants
    - Accessors
  - Constructors / Destructors
    - Canonical Form of Coplien
    - Initialization list
  - Pointers to members
  - Inheritance
    - Abstract classes
- Operator Overloading
- Templates
- Containers
  - Vectors
  - Maps
- Polymorphism (Function Overloading)
  - Subtyping
- Conditional Compilation
- Exceptions
- Casting

### Examples

#### Conditional Compilation

```cpp
#ifdef LOGS
    utl::log(socket, "Closed connection");
#endif
```

#### Classes; Inheritance; Encapsulation; Polymorphism; Canonical Form; Constants

```cpp
#ifndef HEADERS_HPP
# define HEADERS_HPP

# include "ARespComponent.hpp"
# include "ResponseContext.hpp"
# include "MimeTypes.hpp"

class Headers : public ARespComponent {

  public:
      Headers(size_t);
      Headers(std::string const&, size_t, std::string const&, ResponseContext const&, std::string const&);
      Headers(Headers const&);
      ~Headers();
  
      Headers &   operator=(Headers const&);

      size_t      contentLength(void) const;
      void        setContentLength(size_t);

      void        build(void);
      void        build(std::string const&, size_t, std::string const&, ResponseContext const&, std::string const&);

  private:
      Headers();
      size_t      _contentLength;
      MimeTypes   _mimeTypes;

      std::string _findExtension(std::string const& path);
};

#endif
```
