#include "bruco_session.hpp"

BrucoSession::BrucoSession(const int &socket, const std::string &peer_name, const int &buf_size)
	: ProxySession(socket, peer_name, buf_size),
	outbound_key_check_(false), 
	inbound_deny_re_(NULL), outbound_deny_re_(NULL)
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

void BrucoSession::outbound_deny_re(RE2 *re)
{
	outbound_deny_re_ = re;
}

void BrucoSession::inbound_deny_re(RE2 *re)
{
	inbound_deny_re_ = re;
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
