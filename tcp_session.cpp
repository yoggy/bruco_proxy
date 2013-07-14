#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>

#include <pthread.h>

#include "log.hpp"
#include "tcp_server.hpp"
#include "tcp_session.hpp"

int TCPSession::count_ = 0;
Mutex TCPSession::count_mutex_;

TCPSession::TCPSession(const int &socket, const std::string &peer_name, const int &buf_size)
	: socket_(socket), peer_name_(peer_name), buf_size_(buf_size), pt_(0)
{
	buf_ = new char[buf_size];
}

TCPSession::~TCPSession()
{
	if (buf_ != NULL) {
		delete buf_;
		buf_ = NULL;
	}

	dec_count();
}

void *pthread_entry_point_(void *data)
{
	pthread_detach(pthread_self());
	TCPSession *session = (TCPSession *)data;
	session->run();
	return 0;
}

bool TCPSession::start()
{
	log_i("connect : %s", peer_name_.c_str());

	int rv;
	rv = pthread_create(&pt_, NULL, pthread_entry_point_, this);
	if (rv != 0) {
		log_e("pthread_create() failed...");
		finish();
		return false;
	}

	return true;
}

void TCPSession::run()
{
	while(true) {
		memset(buf_, 0, buf_size_);
		size_t read_size = read(socket_, buf_, buf_size_);
		if (read_size < 0) {
			break;
		}
		else if (read_size == 0) {
			break;
		}
		else {
			on_recv(buf_, read_size);
		}
	}

	finish();	
}

void TCPSession::finish()
{
	if (socket_ >= 0) {
		log_i("close : %s", peer_name_.c_str());
		close(socket_);
		socket_ = -1;
	}

	delete this;
}

int TCPSession::send(const char *buf, int buf_size)
{
	return ::send(socket_, buf, buf_size, 0);
}

void TCPSession::on_recv(const char *buf, int buf_size)
{
	// test code...
	log_d("recv : size=%d, data=%s", buf_size, buf);
	send(buf, buf_size);
}

void TCPSession::inc_count()
{
	ScopedLock lock(count_mutex_);
	count_ ++;
}

void TCPSession::dec_count()
{
	ScopedLock lock(count_mutex_);
	count_ --;
}

int TCPSession::get_count()
{
	ScopedLock lock(count_mutex_);
	return count_;
}
