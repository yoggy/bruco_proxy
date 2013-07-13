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

#include <string>
#include <sstream>

#include "log.hpp"


int open_listen_socket(int port)
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

void set_tcp_nodelay(int socket)
{
	int flag;
	flag = 1;
	setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));

#ifdef TCP_CORK
	flag = 0;
	setsockopt(socket, IPPROTO_TCP, TCP_CORK, &flag, sizeof(int));
#endif
}

std::string get_peer_name(const struct sockaddr_storage &sa, const int &sa_len)
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

