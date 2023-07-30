chaque port a :



```C++

int serverSocket;   // plusieurs -> std::vector <int> sockets;
struct sockaddr_in;   // plusieurs car def du port dedans
class epoll { 
        int epollfd    // un seul
        struct epoll_event to_poll; // une seule
        struct epoll_event events [MAX_EVENTS]; // une seule
};

std::map <int, std::string> clients_requests;


```
