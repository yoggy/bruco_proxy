#include "bruco_server.hpp"
#include "bruco_session.hpp"

BrucoServer::BrucoServer() : TCPServer(), cf_(NULL)
{
	cf_ = Config::getInstance();
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

	session->inbound_deny(cf_->get_string("inbound_deny"));
	session->outbound_deny(cf_->get_string("outbound_deny"));

	session->start();
}
