#ifndef ____TCP_HPP____
#define ____TCP_HPP____

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

#include <string>

int open_listen_socket(int port);
void set_tcp_nodelay(int socket);
std::string get_peer_name(const struct sockaddr_storage &sa, const int &sa_len);

#endif // #define ____TCP_HPP____
