#ifndef __BRUCO_SESSION_HPP__
#define __BRUCO_SESSION_HPP__

#include "tcp_session.hpp"

class TCPServer;

class BrucoSession : public TCPSession
{
	public:
		BrucoSession(const int &socket, const std::string &peer_name, const int &buf_size=1024);
		virtual ~BrucoSession();

		virtual bool start();
		virtual void run();
		virtual void finish();

		virtual void on_recv(const char *buf, int buf_size);

	protected:
		int proxy_socket_;
		int proxy_buf_size_;
		char *proxy_buf_;
};

#endif // #define __BRUCO_SESSION_HPP__
