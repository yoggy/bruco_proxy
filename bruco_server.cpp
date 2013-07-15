#include "bruco_server.hpp"
#include "bruco_session.hpp"

BrucoServer::BrucoServer() : TCPServer(), cf_(NULL), inbound_deny_re_(NULL), outbound_deny_re_(NULL)
{
	cf_ = Config::getInstance();
	
	std::string re;

	re = cf_->get_string("inbound_deny");
	if (re != "") {
		inbound_deny_re_ = new RE2(re);
	}

	re = cf_->get_string("outbound_deny");
	if (re != "") {
		outbound_deny_re_ = new RE2(re);
	}
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
	BrucoSession *session = new BrucoSession(socket, peer_name);

	session->outbound_key_check(cf_->get_bool("outbound_key_check"));
	session->key_file(cf_->get_string("key_file"));

	session->inbound_deny_re(inbound_deny_re_);
	session->outbound_deny_re(outbound_deny_re_);

	session->start();
}
