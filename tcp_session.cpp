#include <stdlib.h>
#include "tcp_session.hpp"

TCPSession::TCPSession(const int socket) : socket_(socket), buf_size_(1024)
{
}

TCPSession::~TCPSession()
{
	if (buf_ != NULL) {
		delete buf_;
		buf_ = NULL;
	}
}

int TCPSession::buf_size() const
{
	return buf_size_;
}

void TCPSession::buf_size(const int &val)
{
	buf_size_ = val;
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
