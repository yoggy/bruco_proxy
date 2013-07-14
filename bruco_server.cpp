#include "bruco_server.hpp"
#include "proxy_session.hpp"

BrucoServer::BrucoServer() : TCPServer()
{

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
	ProxySession *session = new ProxySession(socket, peer_name);
	session->start();
}
