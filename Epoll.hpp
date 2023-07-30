#include <sys/epoll.h>
#include <vector>

const int MAX_EVENTS = 10;

class Epoll {

	public:
		Epoll ();
		Epoll (std::vector <int> ports);
		Epoll (Epoll const& rhs);
		~Epoll ();
		Epoll& operator= (Epoll const& rhs);
		
		int		pollPort (int port);
		int		createSocket ();
		void	setReusable (int sockFd);
		void	bindSock (int sockFd);
		void	startListening (int sockFd);
		void	createEpollEvent (int sockFd);

	private:
		int fd;
		struct epoll_event to_poll;
		struct epoll_event events [MAX_EVENTS];
		std::vector <int> sockFds;
};

