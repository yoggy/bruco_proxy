#ifndef __BRUCO_SERVER_HPP__
#define __BRUCO_SERVER_HPP__

#include <string>

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
};

#endif // #define __BRUCO_SERVER_HPP__
