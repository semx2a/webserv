#include <string.h>
/*	strerror*/
#include <unistd.h>
/*	execve
	dup
	dup2
	pipe
	fork
	fcntl
	close
	read
	write
	access
	stat */
#include <fcntl.h>
/* 	fcntl */
#include <stdio.h>
/*	strerror */
#include <sys/types.h>
/*	waitpid
	kill
	gai_strerror
	kqueue
	kevent
	socket
	accept
	send
	listen
	recv
	bind
	getaddrinfo
	freeaddrinfo
	getsockopt
	setsockopt
	stat
	opendir
	closedir */
#include <dirent.h>
/*	opendir
	readdir
	closir */
#include <sys/stat.h>
/* 	stat */
#include <sys/wait.h>
/*	waitpid */
#include <signal.h>
/* 	kill
	signal */
#include <sys/socket.h>
/*	gai_strerror
	socket
	accept
	send
	listen
	recv
	bind
	getaddrinfo
	freeaddrinfo
	getsockopt
	setsockopt */
#include <netdb.h>
/*	gai_strerror */
#include <sys/errno.h>
/*	errno */
#include <arpa/inet.h>
/*	htons
	htonl
	ntohl
	ntohs */
#include <sys/select.h>
/* 	select
	FD_CLR
	FD_SET
	FD_ISSET
	FD_ZERO */

#include <poll.h>
/*	poll */

/* MACOS */
#include <sys/event.h>
/* 	kqueue
	kevent */

/* 	LINUX */
#include <sys/epoll.h>
/* 	epoll
	epoll_create
	epoll_ctl
	epoll_wait */

#include <netdb.h>
/* 	gai_strerror
	getaddrinfo
	freeadrrinfo 
	getprotobyname */

int main()
{
//	EXECUTION
	/* [_] */ execve();
	/* [_] */ dup();
	/* [_] */ dup2();
	/* [_] */ pipe();
	/* [_] */ fork();
	
//	ERRORS	
	/* [_] */ errno;
	/* [_] */ strerror();

//	VALUE CONVERSTION BETWEEN HOST AND NETWORK BYTE ORDER
/* 	These routines convert 16 bit, 32 bit, and 64 bit quantities between
	network byte order and host byte order.
	(Network byte order is big endian, or most significant byte first.)
	On machines which have a byte order which is the same as the
	network order, routines are defined as null macros.
	
	These routines are most often used in conjunction with Internet addresses
	and ports as returned by gethostbyname(3) and getservent(3). */
	/* [_] */ htons;
	/* [_] */ htonl;
	/* [_] */ ntohs;
	/* [_] */ ntohl;

//	SYNCHRONOUS I/O MULTIPLEXING */
	/* [_] */ select();
	/* [_] */ poll(); 			/* examines a set of file
									descriptors to see if some of them are ready for I/O
									or if certain events have occurred on them. */

//	linux	
	/* [_] */ epoll(); 			/* monitoring multiple file descriptors to see if I/O is
									possible on any of them. */
	/* [_] */ epoll_create(); 	/* creates a new epoll instance and returns a file
									descriptor referring to that instance.  */
	/* [_] */ epoll_ctl(); 		/* This system call is used to add, modify, or remove
									entries in the interest list of the epoll(7) instance
									referred to by the file descriptor epfd.
									It requests that the operation op be performed for
									the target file descriptor, fd.  */
	/* [_] */ epoll_wait();		 /* waits for I/O events, blocking the calling thread
									if no events are currently available.  */

//	macOS
	/* [_] */ kqueue(); 		/* The kqueue() system call allocates a kqueue file
									descriptor.  This file descriptor provides a generic
									method of notifying	the user when a kernel event (kevent)
									happens or a condition holds, based on the results of
									small pieces of kernel code termed filters. */

	/* [_] */ kevent(); 		/* sed to register events with the queue, and return
									any pending events to the user. */


//	SOCKETS
	/* [_] */ socket(); 		/* creates an endpoint for communication and returns a descriptor. */
	/* [_] */ accept(); 		/* accept a connection on a socket */
	/* [_] */ listen(); 		/* accept a connection on a socket */
	/* [_] */ send(); 			/* send a message on a socket */
	/* [_] */ recv(); 			/* receive a message on a socket */
	/* [_] */ bind(); 			/* bind a name to a socket */
	/* [_] */ connect(); 		/* initiate a connection on a socket */

//	NETWORK ADRRESS AND SERVICE TRANSLATION
	/* [_] */ getaddrinfo();	/* returns one or more addrinfo structures, each
									of which contains an Internet address that can
									be specified in a call to bind(2) or connect(2).  */
	/* [_] */ freeaddrinfo();	/* frees the memory that was allocated for the
									dynamically allocated linked list res. */
	/* [_] */ gai_strerror(); 	/* returns an error message string corresponding 
									to the error code returned by getaddrinfo(3) or 
									getnameinfo(3).*/
/* 	error codes retuned by getaddrinfo to be read by gai_strerror:
	   EAI_ADDRFAMILY
              The specified network host does not have any network addresses in the requested address family.

       EAI_AGAIN
              The name server returned a temporary failure indication.  Try again later.

       EAI_BADFLAGS
              hints.ai_flags contains invalid flags; or, hints.ai_flags included AI_CANONNAME and name was NULL.

       EAI_FAIL
              The name server returned a permanent failure indication.

       EAI_FAMILY
              The requested address family is not supported.

       EAI_MEMORY
              Out of memory.

       EAI_NODATA
              The specified network host exists, but does not have any network addresses defined.

       EAI_NONAME
              The node or service is not known; or both node and service are NULL; or AI_NUMERICSERV was specified in hints.ai_flags and service was not a numeric port-number string.

       EAI_SERVICE
              The requested service is not available for the requested socket type.  It may be available through another socket type.  For example, this error could occur if service was "shell" (a service available only on stream sockets), and either hints.ai_protocol was IPPROTO_UDP, or hints.ai_socktype was SOCK_DGRAM; or
              the error could occur if service was not NULL, and hints.ai_socktype was SOCK_RAW (a socket type that does not support the concept of services).

       EAI_SOCKTYPE
              The requested socket type is not supported.  This could occur, for example, if hints.ai_socktype and hints.ai_protocol are inconsistent (e.g., SOCK_DGRAM and IPPROTO_TCP, respectively).

       EAI_SYSTEM
              Other system error, check errno for details.

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
*/

//	GET AND SET OPTIONS ON SOCKTETS
	/* [_] */ setsockopt();		/* manipulate options for the socket referred
									to by the file descriptor sockfd.  Options
									may exist at multiple protocol levels; they
									are always present at the uppermost socket level. */
	/* [_] */ getsockname();
	
//	GET PROTOCOL ENTRY
	/* [_] */ getprotobyname();	/* returns a protoent structure for the entry 
									from the database that matches the protocol
									name name.  A connection is opened to the
									database if necessary. */
/*            struct protoent {
               char  *p_name;       //official protocol name
               char **p_aliases;    //alias list
               int    p_proto;      //protocol number
           } 
*/

	/* [_] */ fcntl();
	/* [_] */ close();
	/* [_] */ read();
	/* [_] */ write();
	/* [_] */ waitpid();
	/* [_] */ kill();
	/* [_] */ signal();
	/* [_] */ access();
	
//	FILE STATUS	
	/* [_] */ stat();			/* These functions return information about 
									a file, in the buffer pointed to by statbuf.
									           struct stat {
               dev_t     st_dev;         //ID of device containing file 
               ino_t     st_ino;         //Inode number 
               mode_t    st_mode;        //File type and mode 
               nlink_t   st_nlink;       //Number of hard links 
               uid_t     st_uid;         //User ID of owner 
               gid_t     st_gid;         //Group ID of owner 
               dev_t     st_rdev;        //Device ID (if special file) 
               off_t     st_size;        //Total size, in bytes 
               blksize_t st_blksize;     //Block size for filesystem I/O 
               blkcnt_t  st_blocks;      //Number of 512B blocks allocated 

            	//Since Linux 2.6, the kernel supports nanosecond
            	//precision for the following timestamp fields.
            	//For the details before Linux 2.6, see NOTES.

               struct timespec st_atim;  // Time of last access
               struct timespec st_mtim;  // Time of last modification
               struct timespec st_ctim;  // Time of last status change

           #define st_atime st_atim.tv_sec      //Backward compatibility
           #define st_mtime st_mtim.tv_sec
           #define st_ctime st_ctim.tv_sec
           }; */

	/* [_] */ opendir();
	/* [_] */ readdir();
	/* [_] */ closedir();
}