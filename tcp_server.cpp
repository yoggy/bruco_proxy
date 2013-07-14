#include "log.hpp"
#include "tcp.hpp"
#include "tcp_server.hpp"
#include "tcp_session.hpp"

TCPServer::TCPServer() : listen_port_(12345), socket_(-1), break_flag_(false), max_client_(1000)
{
}

TCPServer::~TCPServer()
{
	stop();
}

int TCPServer::listen_port() const
{
	return listen_port_;
}

void TCPServer::listen_port(const int &val)
{
	listen_port_ = val;
}


int TCPServer::max_client() const
{
	return max_client_;
}

void TCPServer::max_client(const int &val)
{
	max_client_ = val;
}

bool TCPServer::is_start() const
{
	if (socket_ < 0) return false;
	return true;
}

bool TCPServer::start()
{
	int s;
	s = open_listen_socket(listen_port_);
	if (s < 0) return false;
	
	socket_ = s;

	log_i("listen start...port=%d", listen_port_);

	return true;
}

void TCPServer::stop()
{
	if (socket_ != -1) {
		close(socket_);
		socket_ = -1;
	}
}

void TCPServer::run() {
	if (is_start() == false) return;

	struct sockaddr_storage sa;
	socklen_t sa_len;
	int cs;

	sa_len = sizeof(struct sockaddr_storage);

	// main loop
	break_flag_ = false;
	while(!break_flag_) {
		cs = accept(socket_, (struct sockaddr*) &sa, &sa_len);
		set_tcp_nodelay(cs);

		if (cs > 0) {
			if (max_client_ > TCPSession::get_count()) {
				TCPSession::inc_count();
				on_accept(cs, get_peer_name(sa, sa_len));
			}
			else {
				log_w(
					"REFUSE CLIENT CONNECTION! max_client=%d, peer=%s",
					TCPSession::get_count(),
					get_peer_name(sa, sa_len).c_str());
				close(cs);
			}
		}
	}
}

void TCPServer::on_accept(const int &socket, const std::string &peer_name)
{
	TCPSession *session = new TCPSession(socket, peer_name);
	session->start();
}


