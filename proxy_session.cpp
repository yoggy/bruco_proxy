#include "proxy_session.hpp"
#include "log.hpp"
#include "tcp.hpp"

ProxySession::ProxySession(std::string &proxy_host, const int &proxy_port, const int &socket, const std::string &peer_name, const int &buf_size) 
	: TCPSession(socket, peer_name, buf_size),
	proxy_host_(proxy_host), proxy_port_(proxy_port),
	proxy_socket_(-1), proxy_buf_size_(buf_size), break_flag_(false)
{
	proxy_buf_ = new char[proxy_buf_size_];
}

ProxySession::~ProxySession()
{
	if (proxy_buf_ != NULL) {
		delete proxy_buf_;
		proxy_buf_ = NULL;
	}
}

void ProxySession::break_session()
{
	log_w("break_session() from=%s", peer_name_.c_str());
	break_flag_ = true;
}

bool ProxySession::start()
{
	proxy_socket_ = connect_tcp(proxy_host_.c_str(), proxy_port_);
	if (proxy_socket_ < 0) {
		finish();
		return false;
	}

	return TCPSession::start();
}

void ProxySession::run()
{
	fd_set fds;

	FD_ZERO(&fds);
	FD_SET(socket_, &fds);
	FD_SET(proxy_socket_, &fds);
	int max_fd = std::max(socket_, proxy_socket_) + 1;

	break_flag_ = false;
	while(!break_flag_) {
		fd_set work_fds = fds;
		int rv = select(max_fd, &work_fds, NULL, NULL, NULL);
		if (rv == 0) break; 
	
		// from client
		if (FD_ISSET(socket_, &work_fds) && break_flag_ == false) {
			memset(buf_, 0, buf_size_);
			int read_size = recv(socket_, buf_, buf_size_, 0);
			if (read_size <= 0) {
				break;
			}
			else {
				on_recv(buf_, read_size);
			}
		}

		// from server
		if (FD_ISSET(proxy_socket_, &work_fds) && break_flag_ == false) {
			memset(proxy_buf_, 0, proxy_buf_size_);
			int read_size = recv(proxy_socket_, proxy_buf_, proxy_buf_size_, 0);
			if (read_size <= 0) {
				break;
			}
			else {
				on_recv_proxy(proxy_buf_, read_size);
			}
		}
	}

	finish();
}

void ProxySession::finish()
{
	if (proxy_socket_ >= 0) {
		close(proxy_socket_);
		proxy_socket_ = -1;
	}

	TCPSession::finish();
}

void ProxySession::on_recv(const char *buf, int buf_size)
{
	// log_d("on_recv() : size=%d, data=%s", buf_size, buf);

	// client -> proxy -> server
	send_proxy(buf, buf_size);
}

int ProxySession::send_proxy(const char *buf, int buf_size)
{
	return ::send(proxy_socket_, buf, buf_size, 0);
}

void ProxySession::on_recv_proxy(const char *buf, int buf_size)
{
	//log_d("on_recv_proxy() : size=%d, data=%s", buf_size, buf);

	// server -> proxy -> client
	send(buf, buf_size);
}

