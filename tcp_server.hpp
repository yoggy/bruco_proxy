#ifndef __TCP_SERVER_HPP__
#define __TCP_SERVER_HPP__

class TCPServer
{
	public:
		TCPServer();
		virtual ~TCPServer();

		int listen_port() const;
		void listen_port(const int &val);	

		bool start();

	private:
		int listen_port_;
		int socket_;
};


#endif // #define __TCP_SERVER_HPP__
