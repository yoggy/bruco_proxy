#include "bruco_server.hpp"
#include "bruco_session.hpp"

BrucoServer::BrucoServer() : TCPServer(), cf_(NULL), inbound_deny_re_(NULL), outbound_deny_re_(NULL)
{
	cf_ = Config::getInstance();
	
	std::string re;

	re = cf_->get_string("inbound_deny");
	if (re != "") {
		inbound_deny_re_ = new RE2(re, RE2::Latin1);
	}

	re = cf_->get_string("outbound_deny");
	if (re != "") {
		outbound_deny_re_ = new RE2(re, RE2::Latin1);
	}

	proxy_host_ = cf_->get_string("forward_host");
	proxy_port_ = cf_->get_int("forward_port");
}

BrucoServer::~BrucoServer()
{

}

bool BrucoServer::start()
{
	return TCPServer::start();
}

void BrucoServer::on_accept(const int &socket, const std::string &peer_name)
{
	BrucoSession *session = new BrucoSession(proxy_host_, proxy_port_, socket, peer_name);

	session->outbound_key_check(cf_->get_bool("outbound_key_check"));
	session->outbound_key_check_xor256(cf_->get_bool("outbound_key_check_xor256"));
	session->outbound_key_file(cf_->get_string("outbound_key_file"));

	session->inbound_jmpcall_check(cf_->get_bool("inbound_jmpcall_check"));

	session->inbound_deny_re(inbound_deny_re_);
	session->outbound_deny_re(outbound_deny_re_);

	session->dump_stream(cf_->get_bool("dump_stream"));

	session->start();
}
