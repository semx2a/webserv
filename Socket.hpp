
const int MAX_EVENTS = 10;

class Socket {

	public:
		Socket (int port);

		void	createSocket ();
		void	setReusable ();
		void	setServerAddr ();
		void	bindSock ();
		void	startListening ();

		int		getFd () const { return _sockFd; } //TODO implement in .cpp

	private:
		int					_sockFd;
		int					_port;
		struct sockaddr_in	_serverAddr;
		struct epoll_event	_to_poll;
		struct epoll_event	_events [MAX_EVENTS];
};

