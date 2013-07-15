#ifndef __BRUCO_SERVER_HPP__
#define __BRUCO_SERVER_HPP__

#include <string>
#include <re2/re2.h>

#include "config.hpp"
#include "tcp_server.hpp"

class BrucoServer : public TCPServer
{
	public:
		BrucoServer();
		virtual ~BrucoServer();

		virtual bool start();
		virtual void on_accept(const int &socket, const std::string &peer_name);

	private:
		Config *cf_;

		RE2 *inbound_deny_re_;
		RE2 *outbound_deny_re_;
};

#endif // #define __BRUCO_SERVER_HPP__
