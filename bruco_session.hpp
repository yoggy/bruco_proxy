#ifndef __BRUCO_SESSION_HPP__
#define __BRUCO_SESSION_HPP__

#include "proxy_session.hpp"

class BrucoSession : public ProxySession
{
	public:
		BrucoSession(const int &socket, const std::string &peer_name, const int &buf_size=1024);
		virtual ~BrucoSession();

		bool outbound_key_check() const;
		void outbound_key_check(const bool &flag);
		std::string key_file() const;
		void key_file(const std::string &val);
		std::string outbound_deny() const;
		void outbound_deny(const std::string &val);
		std::string inbound_deny() const;
		void inbound_deny(const std::string &val);

		virtual bool start();

		virtual void on_recv(const char *buf, int buf_size);
		virtual void on_recv_proxy(const char *buf, int buf_size);

	protected:
		bool outbound_key_check_;
		std::string key_file_;
		std::string outbound_deny_;
		std::string inbound_deny_;
};

#endif // #define __BRUCO_SESSION_HPP__
