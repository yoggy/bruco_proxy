#ifndef __TCP_SESSION_HPP__
#define __TCP_SESSION_HPP__

class TCPSession {
	public:
		TCPSession(int socket);
		virtual ~TCPSession();
		
		int buf_size() const;
		void buf_size(const int &val);

		virtual bool start();

		virtual void send(const char *buf, int buf_size);
		virtual void on_recv(const char *buf, int buf_size);


	protected:
		virtual void run_();

	protected:
		int socket_;
		int buf_size_;
		char *buf_;
};
#endif // #define __TCP_SESSION_HPP__

