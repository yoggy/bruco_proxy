#ifndef __PROXY_SESSION_HPP__
#define __PROXY_SESSION_HPP__

#include "tcp_session.hpp"

class TCPServer;

class ProxySession : public TCPSession
{
	public:
		ProxySession(std::string &proxy_host, const int &proxy_port, const int &socket, const std::string &peer_name, const int &buf_size=1024);
		virtual ~ProxySession();

		void break_session();

		virtual bool start();
		virtual void run();
		virtual void finish();

		virtual void on_recv(const char *buf, int buf_size);

		virtual int  send_proxy(const char *buf, int buf_size);
		virtual void on_recv_proxy(const char *buf, int buf_size);

	protected:
		std::string proxy_host_;
		int proxy_port_;
		int proxy_socket_;
		int proxy_buf_size_;
		char *proxy_buf_;
		bool break_flag_;

		int inbound_count_;
		int outbound_count_;
		int inbound_total_bytes_;
		int outbound_total_bytes_;
};

#endif // #define __PROXY_SESSION_HPP__
