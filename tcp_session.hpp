#ifndef __TCP_SESSION_HPP__
#define __TCP_SESSION_HPP__

class TCPSession {
	public:
		TCPSession(int socket);
		virtual bool start();

	protected:
		int socket_;
};
#endif // #define __TCP_SESSION_HPP__

