/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 14:55:43 by seozcan           #+#    #+#             */
/*   Updated: 2023/07/26 10:11:46 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/* 
struct addrinfo {
	int              ai_flags;
	int              ai_family;
	int              ai_socktype;
	int              ai_protocol;
	socklen_t        ai_addrlen;
	struct sockaddr *ai_addr;
	char            *ai_canonname;
	struct addrinfo *ai_next;
};

The sockaddr_storage structure is a sockaddr that is not associated with an
address family.  Instead, it is large enough to hold the contents of any of
the other sockaddr structures.  It can be used to embed sufficient storage
for a sockaddr of any type within a larger structure.

struct sockaddr_storage {
	sa_family_t	ss_family;
}

struct sockaddr {
	sa_family_t sa_family;
	char        sa_data[];
};

*/

 #define BUFFER_SIZE 1024

class Webserv {
	
	public:
		
		Webserv(void);
		Webserv(int port);
		Webserv(Webserv const & src);
		~Webserv(void);

		Webserv &			operator=(Webserv const & rhs);

		int					getPort(void) const;
		int					getSocket(void) const;
		const char *		getBuffer(void) const;

		void				setPort(const int);
		void				setSocket(const int);
		void				setBuffer(const char *);
		
		void				run(void);
		
	private:
		
 		int 				_port;
    	int 				_serverSocket;
    	char 				_buffer[BUFFER_SIZE];

		void 				_createSocket();
    	void 				_bindSocket();
    	void 				_listenForConnections();
    	void 				_handleConnection(int clientSocket);

};

std::ostream &	operator<<(std::ostream & o, Webserv const & w);