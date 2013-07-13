#include "bruco_server.hpp"
#include "bruco_session.hpp"

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
	BrucoSession *session = new BrucoSession(socket, peer_name);
	session->start();
}
