/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seozcan <seozcan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 14:56:51 by seozcan           #+#    #+#             */
/*   Updated: 2023/07/26 14:07:40 by seozcan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Webserv.hpp"
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: CONSTRUCTORS::

Webserv::Webserv(void) : _buffer() { };

Webserv::Webserv(int port) : _port(port), _buffer() {
	
	this->_createSocket();
	this->_bindSocket();
}

Webserv::~Webserv() { };

//::::::::::::::::::::::::::::::::::::::::::::::::::::::: COMPARISON OPERATORS::

Webserv &	    Webserv::operator=(Webserv const & rhs) {
	
	if (this != &rhs) {
		
		this->setPort(rhs.getPort());
		this->setSocket(rhs.getSocket());
		this->setBuffer(rhs.getBuffer());
	}
	
	return *this;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: ACCESSORS::

int				Webserv::getPort(void) const { return this->_port; }

int				Webserv::getSocket(void) const { return this->_serverSocket; }

const char *	Webserv::getBuffer(void) const { return this->_buffer; }

void			Webserv::setPort(const int port) { this->_port = port; }

void			Webserv::setSocket(const int socket) { this->_serverSocket = socket; }

void			Webserv::setBuffer(const char * str) { 
	
	strncpy(this->_buffer, str, BUFFER_SIZE - 1);
	this->_buffer[BUFFER_SIZE - 1] = '\0';

}

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: MEMBER FUNCTIONS::

void			Webserv::_createSocket() {
	
	this->setSocket(socket(AF_INET, SOCK_STREAM, 0));
	if (this->getSocket() == 0) {
		
		std::cerr << "Socket creation failed." << std::endl;
		exit(1);
	}
}

void			Webserv::_bindSocket() {
	
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(this->getPort());

	if (bind(this->getSocket(), (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
		
		std::cerr << "Binding failed." << std::endl;
		exit(1);
	}
}

void    		Webserv::_listenForConnections() {
	
	if (listen(this->getSocket(), 5) < 0) {
		
		std::cerr << "Listening failed." << std::endl;
		exit(1);
	}
}

void	 	   Webserv::_handleConnection(int clientSocket) {
	
	int bytesRead = read(clientSocket, (void *)this->getBuffer(), BUFFER_SIZE);
	if (bytesRead < 0) {
		
		std::cerr << "Reading from client failed." << std::endl;
		close(clientSocket);
		return;
	}

	std::cout << "Received data from client: " << this->getBuffer() << std::endl;

	std::string response = "Hello, Client!";
	send(clientSocket, response.c_str(), response.length(), 0);

	close(clientSocket);
}

void    		Webserv::run() {
	
	this->_listenForConnections();

	while (true) {
		
		int newSocket;
		struct sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);

		if ((newSocket = accept(this->getSocket(), (struct sockaddr *)&clientAddr, &clientAddrLen)) < 0) {
			std::cerr << "Accepting connection failed." << std::endl;
			continue;
		}

		this->_handleConnection(newSocket);
	}
}

// ::::::::::::::::::::::::::::::::::::::::::::::: OUTPUT OPERATOR OVERLOADING::

std::ostream &	operator<<(std::ostream & o, Webserv const & w) {
	
	(void)w;
	return o << "hello webserv" << std::endl;
};
