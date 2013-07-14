#include "bruco_session.hpp"


BrucoSession::BrucoSession(const int &socket, const std::string &peer_name, const int &buf_size)
	: ProxySession(socket, peer_name, buf_size)
{

}

BrucoSession::~BrucoSession()
{
	
}

bool BrucoSession::outbound_key_check() const
{
	return outbound_key_check_;
}

void BrucoSession::outbound_key_check(const bool &flag)
{
	outbound_key_check_ = flag;
}

std::string BrucoSession::key_file() const
{
	return key_file_;
}

void BrucoSession::key_file(const std::string &val)
{
	key_file_ = val;
}

std::string BrucoSession::outbound_deny() const
{
	return outbound_deny_;
}

void BrucoSession::outbound_deny(const std::string &val)
{
	outbound_deny_ = val;
}

std::string BrucoSession::inbound_deny() const
{
	return inbound_deny_;
}

void BrucoSession::inbound_deny(const std::string &val)
{
	inbound_deny_ = val;
}

bool BrucoSession::start()
{
	return ProxySession::start();
}

void BrucoSession::on_recv(const char *buf, int buf_size)
{
	ProxySession::on_recv(buf, buf_size);
}

void BrucoSession::on_recv_proxy(const char *buf, int buf_size)
{
	ProxySession::on_recv_proxy(buf, buf_size);
}
