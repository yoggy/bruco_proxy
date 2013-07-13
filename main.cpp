#include <string>
#include <iostream>
#include <stdlib.h>

#include "log.hpp"
#include "bruco_server.hpp"

void usage() 
{
	std::cerr << "usage : ./bruco_proxy rule_file listen_port forward_host forward_port" << std::endl;
	std::cerr << std::endl;
	std::cerr << "    examples" << std::endl;
	std::cerr << std::endl;
	std::cerr << "            $ ./bruco_proxy test.rule 12345 localhost 23456" << std::endl;
	std::cerr << std::endl;
	std::cerr << std::endl;

	exit(1);
}

int main(int argc, char *argv[])
{
	if (argc != 5) usage();

	std::string rule_file    = argv[1];
	int         listen_port  = atoi(argv[2]);
	std::string forward_host = argv[3];
	int         forward_port = atoi(argv[4]);

	std::cerr << "==== startup bruco_proxy ====" << std::endl;
	log_d(
		"rule_file=%s, listen_port=%d, forward_host=%s, forward_port=%d", 
		rule_file.c_str(), listen_port, forward_host.c_str(), forward_port);

	BrucoServer server;
	server.listen_port(listen_port);

	if (server.start() == false) {
		log_e("error: server.start() failed...");
		return -1;
	}

	server.run();

	return 0;
}

