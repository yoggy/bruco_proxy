#include <stdlib.h>
#include "tcp_session.hpp"

TCPSession::TCPSession(const int &socket, const int &buf_size) : socket_(socket), buf_size_(buf_size)
{
	buf_ = new char[buf_size];
}

TCPSession::~TCPSession()
{
	if (buf_ != NULL) {
		delete buf_;
		buf_ = NULL;
	}
}

bool TCPSession::start()
{
	return true;
}

void TCPSession::run_()
{
}

void TCPSession::send(const char *buf, int buf_size)
{

}

void TCPSession::on_recv(const char *buf, int buf_size)
{

}
