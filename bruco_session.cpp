#include "bruco_session.hpp"
#include "log.hpp"

BrucoSession::BrucoSession(const int &socket, const std::string &peer_name, const int &buf_size) 
	: TCPSession(socket, peer_name, buf_size), proxy_socket_(-1), proxy_buf_size_(buf_size)
{
	proxy_buf_ = new char[proxy_buf_size_];
}

BrucoSession::~BrucoSession()
{
	if (proxy_buf_ != NULL) {
		delete proxy_buf_;
		proxy_buf_ = NULL;
	}
}

bool BrucoSession::start()
{
	log_d("BrucoSession::start()");
	return TCPSession::start();
}

void BrucoSession::run()
{
	log_d("BrucoSession::run()");
	TCPSession::run();
}

void BrucoSession::finish()
{
	log_d("BrucoSession::finish()");
	TCPSession::finish();
}

void BrucoSession::on_recv(const char *buf, int buf_size)
{
	log_d("BrucoSession::on_recv()");
	TCPSession::on_recv(buf, buf_size);
}
