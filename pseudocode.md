chaque port a :



```C++

int serverSocket;
struct sockaddr_in;
class epoll { 
        int epollfd
        struct epoll_event to_poll;
        struct epoll_event events [MAX_EVENTS];
};

std::map <int, std::string> clients_requests;


```
