#include <cerrno>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

const int MAX_EVENTS = 10;
const int BUFFER_SIZE = 1024;

int main ()
{
	try
	{
		int serverSocket;
		struct sockaddr_in serverAddress;

/*
**	Creation du socket serveur : creation d'un socket TCP/IP en utilisant l'appel systeme socket ()		
**	Ce socket sera utilise pour ecouter les nouvelles connexions entrantes.								
**	int socket (int domain, int type, int protocol);														
**	AF_INET = IPv4																						
**	SOCK_STREAM = socket oriente connection																
**	0 = utilisation du protocol par default pour le domaine et le type specifies							
*/

		serverSocket = socket (AF_INET, SOCK_STREAM, 0);
		if (serverSocket == -1)
			throw std::runtime_error ("socket (): " + (std::string) strerror (errno));

/*
**	L'option SO_REUSEADDR permet a un socket recemment ferme d'etre reutilise immediatement.				
**	Signifie qu'on peut lier un socket a une adresse et a un port meme si une connexion					
**	precedente associee a cette adresse et a ce port n'est pas encore completement fermee.				
**	/!\ a utiliser avec prudence (juste pour DEBUG maybe)												
*/

    	int yes = 1;
    	setsockopt (serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (int));

/*
**	Configuration de l'adresse du serveur en utilisant une struct `sockaddr_in` :						 
**	Specification du numero de port (8080) sur lequel le serveur ecoutera les connections entrantes		
**	INADDR_ANY : constante speciale qui represente toutes les adresses IP disponibles sur la machine où	 
**	s'execute le programme du serveur. Qd utilisee dans la configuration de l'adresse IP du serveur,		 
**	cela signifie que le serveur ecoutera sur toutes les interfaces reseau de la machine.				
*/

    	serverAddress.sin_family = AF_INET;
    	serverAddress.sin_port = htons (8080);  // Port 8080
    	serverAddress.sin_addr.s_addr = htonl (INADDR_ANY);

/*
**	Liaison du socket a l'adresse du serveur :															
**	Utilisation de l'appel systeme `bind ()`. Le serveur ecoutera les connections sur cette addresse		
*/

    	if (bind (serverSocket, (struct sockaddr*)&serverAddress, sizeof (serverAddress)) == -1)
    	{
    		close (serverSocket);
    		throw std::runtime_error ("bind (): " + (std::string) strerror (errno));
    	}

/*
**	Mise en ecoute du socket:																			
**	L'appel systeme `listen ()` est utilise pour mettre le serveur socket en ecoute.						
**	Permet au serveur d'accepter les connections entrantes												
*/

    	if (listen (serverSocket, 5) == -1)
    	{
    		close (serverSocket);
    		throw std::runtime_error ("listen (): " + (std::string) strerror (errno));
    	}

/*
**	L'instance epoll est un mecanisme -gere par le noyau Linux- qui surveille plusieurs descripteurs		 
**	de fichiers (sockets, fichiers, etc.) et detecte les evenements d'I/O associes a ces descripteurs.	 
**	Il permet la gestion efficace des operations d'I/O non bloquantes et evite d'avoir a parcourir		 
**	manuellement les descripteurs de fichiers pour verifier leur etat.									 
**	Lorsqu'on cree une nouvelle instance epoll en utilisant l'appel systeme epoll_create1 (), le noyau	 
**	alloue des ressources et renvoie un fd representant cet 'evenement' ou 'instance' epoll.				
**	Ce descripteur de fichier est utilise pour faire reference a l'instance en question dans les appels	 
**	ulterieurs a l'API epoll (ex: ajout ou suppression de descripteurs de fichiers a surveiller,			 
**	modification des evenements a surveiller pour un fd donne, attente des evenements d'I/O.)			
*/

    	int epollFd = epoll_create1 (0);
    	if (epollFd == -1)
    	{
    		close (serverSocket);
    		throw std::runtime_error ("epoll_create1 (): " + (std::string) strerror (errno));
    	}

/*
**	Configuration de l'instance pour surveiller les evenements d'I/O souhaites sur un descripteur de		 
**	fichier specifique :																					
**	On cree une struct epoll_event et on y remplit les infos necessaires avt de l'enregistrer en tant	
**	qu'instance epoll en utilisant la fonction epoll_ctl ().												
**	 																									
**	- to_poll.events : indique les types d'evenements a surveiller pour le fd specifie
**	Ici, il est configure avec la valeur EPOLLIN, qui signifie un evenement d'entree (lecture) disponible 
**	sur le descripteur de fichier.																		
**	 																									
**	- to_poll.data.fd : specifie le descripteur de fichier auquel l'instance epoll est associee
**	et donc le fd dont elle surveillera les evenements d'I/O												
*/

    	struct epoll_event to_poll;
    	to_poll.events = EPOLLIN; 
    	to_poll.data.fd = serverSocket;  

/*
**	Enregistrement du socket serveur en tant que fd a surveiller par epoll								
**	Seuls les entrees seront surveillees (cf definition de events : EPOLLIN)								
*/

    	if (epoll_ctl (epollFd, EPOLL_CTL_ADD, serverSocket, &to_poll) == -1)
    	{
    		close (serverSocket);
    		close (epollFd);
    		throw std::runtime_error ("epoll_ctl (): " + (std::string) strerror (errno));
    	}

/*
**	Creation d'un tableau de structures que epoll_wait viendra remplir en cas d'evements detectes sur le 
**	fd du socket du serveur																				
*/

    	struct epoll_event events [MAX_EVENTS];

/*
**	Boucle principale: epoll_wait () attend qu'un/des evenement.s se produise.nt							
**	Lorsqu'un evenement se produit, il est traite en fonction de son type :								
**	- nouvelle connection entrante																		
**	- donnees disponibles a lire																			
*/

		int	clientSocket;
		struct sockaddr_in clientAddress;
		socklen_t clientAddressSize = sizeof (clientAddress);

		while (true)
		{
			int numEvents = epoll_wait (epollFd, events, MAX_EVENTS, -1);
			if (numEvents == -1)
			{
				close (serverSocket);
				close (epollFd);
				throw std::runtime_error ("epoll_wait (): " + (std::string) strerror (errno));
			}

			// Parcourir tous les evenements
			for (int i = 0; i < numEvents; ++i)
			{
				if (events [i].data.fd == serverSocket)
				{
					// Nouvelle connexion entrante --> creation d'un socket specifique a ce client
					clientSocket = accept (serverSocket, (struct sockaddr*)&clientAddress, &clientAddressSize);
					if (clientSocket == -1)
					{
						close (serverSocket);
						close (epollFd);
						throw std::runtime_error ("accept (): " + (std::string) strerror (errno));
					}

					std::cout << "Nouvelle connexion entrante : " << inet_ntoa (clientAddress.sin_addr) << std::endl;

					// Ajout du nouveau socket client a la liste des fd a surveiller
					to_poll.events = EPOLLIN;
					to_poll.data.fd = clientSocket;
					if (epoll_ctl (epollFd, EPOLL_CTL_ADD, clientSocket, &to_poll) == -1)
					{
						close (clientSocket);
						throw std::runtime_error ("epoll_ctl (): " + (std::string) strerror (errno));
					}
				}
				else
				{
					// Donnees disponibles a lire sur un socket client existant
					char buffer [BUFFER_SIZE];
					int bytesRead = read (events [i].data.fd, buffer, BUFFER_SIZE - 1);								// note: mieux d'utiliser recv ?
					if (bytesRead == -1)
					{
						close (events [i].data.fd);
						throw std::runtime_error ("read (): " + (std::string) strerror (errno));					// note: A SUPPR A TERME CAR INTERDIT DANS SUJET
					}
					else if (bytesRead == 0)
					{
						std::cout << "Connexion terminee" << std::endl;
						close (events [i].data.fd);
					}
					else
					{
						buffer [bytesRead] = '\0';
						std::cout << "Donnees reçues : " << buffer << std::endl;
						send (events [i].data.fd, "END", 3, 0);
					}
				}
			}
		}
		close (serverSocket);
		close (epollFd);
	}
	catch (std::exception& e)
	{
		std::cerr << "Server: Error: " << e.what () << std::endl;
		return 1;
	}
	return 0;
}

