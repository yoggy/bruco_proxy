#ifndef __PROXY_SESSION_HPP__
#define __PROXY_SESSION_HPP__

#include "tcp_session.hpp"

class TCPServer;

class ProxySession : public TCPSession
{
	public:
		ProxySession(const int &socket, const std::string &peer_name, const int &buf_size=1024);
		virtual ~ProxySession();

		void break_session();

		virtual bool start();
		virtual void run();
		virtual void finish();

		virtual void on_recv(const char *buf, int buf_size);

		virtual int  send_proxy(const char *buf, int buf_size);
		virtual void on_recv_proxy(const char *buf, int buf_size);


	protected:
		int proxy_socket_;
		int proxy_buf_size_;
		char *proxy_buf_;

		bool break_flag_;
};

#endif // #define __PROXY_SESSION_HPP__
