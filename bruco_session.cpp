#include "bruco_session.hpp"
#include "log.hpp"

BrucoSession::BrucoSession(std::string &proxy_host, const int &proxy_port, const int &socket, const std::string &peer_name, const int &buf_size)
	: ProxySession(proxy_host, proxy_port, socket, peer_name, buf_size),
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
	log_d("BrucoSession::on_recv()");
	if (inbound_deny_re_ != NULL) {
		std::string target_str(buf, buf_size);
		bool rv = RE2::PartialMatch(target_str, *inbound_deny_re_);
		if (rv) {
			log_w("break_session() : %s, inbound_re=%s", peer_name_.c_str(), inbound_deny_re_->pattern().c_str());
			break_session();
			return;
		}
	}

	ProxySession::on_recv(buf, buf_size);
}

void BrucoSession::on_recv_proxy(const char *buf, int buf_size)
{
	if (outbound_deny_re_ != NULL) {
		std::string target_str(buf, buf_size);
		bool rv = RE2::PartialMatch(target_str, *outbound_deny_re_);
		if (rv) {
			log_w("break_session() : %s, outbound_re=%s", peer_name_.c_str(), outbound_deny_re_->pattern().c_str());
			break_session();
			return;
		}
	}

	ProxySession::on_recv_proxy(buf, buf_size);
}
