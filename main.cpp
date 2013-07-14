#include <string>
#include <iostream>
#include <stdlib.h>

#include "log.hpp"
#include "config.hpp"
#include "bruco_server.hpp"

void usage() 
{
	std::cerr << "usage : ./bruco_proxy config_file"  << std::endl;
	std::cerr << std::endl;
	std::cerr << "    examples" << std::endl;
	std::cerr << std::endl;
	std::cerr << "            $ ./bruco_proxy sample_config.txt" << std::endl;
	std::cerr << std::endl;
	std::cerr << std::endl;

	exit(1);
}

int main(int argc, char *argv[])
{
	if (argc != 2) usage();

	std::string config_file = argv[1];
	if (Config::load(config_file) == false) {
		log_e("config_file load failed....config_file=%s", config_file.c_str());
		return -1;
	}

	Config *cf = Config::getInstance();

	int         listen_port  = cf->get_int("listen_port");
	std::string forward_host = cf->get_string("forward_host");
	int         forward_port = cf->get_int("forward_port");
	int         max_client   = cf->get_int("max_client");

	std::cerr << "==== startup bruco_proxy ====" << std::endl;
	log_d(
		"config_file=%s, listen_port=%d, forward_host=%s, forward_port=%d", 
		config_file.c_str(), listen_port, forward_host.c_str(), forward_port);

	BrucoServer server;
	server.listen_port(listen_port);
	server.max_client(max_client);

	if (server.start() == false) {
		log_e("error: server.start() failed...");
		return -1;
	}

	server.run();

	return 0;
}

