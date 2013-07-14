#ifndef __TCP_SERVER_HPP__
#define __TCP_SERVER_HPP__

#include <string>
#include "lock.hpp"

class TCPServer
{
	public:
		TCPServer();
		virtual ~TCPServer();

		int listen_port() const;
		void listen_port(const int &val);	

		int max_client() const;
		void max_client(const int &val);

		bool is_start() const;

		virtual bool start();
		virtual void stop();

		virtual void run();
		virtual void on_accept(const int &socket, const std::string &peer_name);

	private:
		int listen_port_;
		int socket_;
		bool break_flag_;
		
		int max_client_;
};

#endif // #define __TCP_SERVER_HPP__
