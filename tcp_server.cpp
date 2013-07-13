#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <sstream>

#include "log.hpp"
#include "tcp_server.hpp"
#include "tcp_session.hpp"

int open_listen_socket_(int port)
{
	int ss;
	int rv;
	int flag;
	struct addrinfo hints;
	struct addrinfo *res = NULL;
	struct addrinfo *ai = NULL;
	struct addrinfo *ai_inet = NULL;
	struct addrinfo *ai_inet6 = NULL;

	char port_str[16];
	snprintf(port_str, 15, "%d", port);

	// get addrinfo
	memset((void*)(&hints), 0, sizeof(struct addrinfo));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags    = AI_PASSIVE;

	rv = getaddrinfo(NULL, port_str, &hints, &res);
	if (rv != 0) {
		log_e("getaddrinfo() failed...%s", gai_strerror(rv));
		return -1;
	}

	for (ai = res; ai != NULL; ai = ai->ai_next) {
		if (ai->ai_family == AF_INET ) ai_inet  = ai;
		if (ai->ai_family == AF_INET6) ai_inet6 = ai;
	}

	if (ai_inet == NULL && ai_inet6 == NULL) {
		log_e("getaddrinfo() return null results...");
		freeaddrinfo(res);
		return -1;
	}

	// socket (try ipv6 before ipv4)
	ss = -1;
	if (ai_inet6 != NULL) {
		ss = socket(ai_inet6->ai_family, ai_inet6->ai_socktype, ai_inet6->ai_protocol);
		ai = ai_inet6;
	}
	if (ai_inet != NULL && ss < 0) {
		ss = socket(ai_inet->ai_family, ai_inet->ai_socktype, ai_inet->ai_protocol);
		ai = ai_inet;
	}
	if (ss < 0) {
		log_e("socket() failed...");
		freeaddrinfo(res);
		return -1;
	}

	// reuse address
	flag = 1;
	rv = setsockopt(ss, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
	if (rv < 0) {
		log_e("setsockopt() failed...");
		freeaddrinfo(res);
		return -1;
	}

	// bind
	rv = bind(ss, ai->ai_addr, ai->ai_addrlen);
	if (rv < 0) {
		log_e("bind() failed...");
		freeaddrinfo(res);
		return -1;
	}

	// listen
	rv = listen(ss, 5);
	if (rv < 0) {
		log_e("listen() failed...");
		freeaddrinfo(res);
		return -1;
	}

	freeaddrinfo(res);

	return ss;
}

std::string get_peer_name_(const struct sockaddr_storage &sa, const int &sa_len)
{
	char addr_str[NI_MAXHOST];
	char serv_str[NI_MAXSERV];

	getnameinfo(
			(struct sockaddr *)&sa, sa_len,
            addr_str,
			NI_MAXHOST,
			serv_str,
			NI_MAXSERV,
            NI_NUMERICHOST | NI_NUMERICSERV);

	std::stringstream ss;
	ss << "addr=" << addr_str << ", port=" << serv_str;
	return ss.str();
}

TCPServer::TCPServer() : listen_port_(12345), socket_(-1), break_flag_(false)
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

bool TCPServer::is_start() const
{
	if (socket_ < 0) return false;
	return true;
}

bool TCPServer::start()
{
	int s;
	s = open_listen_socket_(listen_port_);
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

		int flag;
		flag = 1;
		setsockopt(cs, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));

#ifdef TCP_CORK
		flag = 0;
		setsockopt(cs, IPPROTO_TCP, TCP_CORK, &flag, sizeof(int));
#endif

		if (cs > 0) {
			on_accept(cs, get_peer_name_(sa, sa_len));
		}
	}
}

void TCPServer::on_accept(const int &socket, const std::string &peer_name)
{
	TCPSession *session = new TCPSession(socket, peer_name);
	session->start();
}
