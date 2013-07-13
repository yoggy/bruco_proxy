#ifndef __TCP_SESSION_HPP__
#define __TCP_SESSION_HPP__

#include <string>

class TCPSession {
	public:
		TCPSession(const int &socket, const std::string &peer_name, const int &buf_size=1024);
		virtual ~TCPSession();
		
		virtual bool start();
		virtual void run();
		virtual void finish();

		virtual int send(const char *buf, int buf_size);
		virtual void on_recv(const char *buf, int buf_size);

	protected:

	protected:
		int socket_;
		std::string peer_name_;
		int buf_size_;
		char *buf_;
		pthread_t pt_;
};
#endif // #define __TCP_SESSION_HPP__

