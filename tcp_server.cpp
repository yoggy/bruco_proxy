#include "tcp_server.hpp"

TCPServer::TCPServer() : listen_port_(12345), socket_(-1)
{
}

TCPServer::~TCPServer()
{
}

int TCPServer::listen_port() const
{
	return listen_port_;
}

void TCPServer::listen_port(const int &val)
{
	listen_port_ = val;
}

bool TCPServer::start()
{
	return false;
}

