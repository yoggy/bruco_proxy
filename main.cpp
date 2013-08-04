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

	// nice setting
	if (cf->has_key("nice")) {
		int nice_val = cf->get_int("nice");
		nice(nice_val);
	}

	// logging settings
	set_output_log_level(cf->get_int("output_log_level"));
	set_output_string_length(cf->get_int("output_string_length"));
	if (cf->get_bool("udp_log_output_enable")) {
		start_udp_log_output(
				cf->get_string("udp_log_output_host").c_str(),
				cf->get_int("udp_log_output_port")
				);
	}

	// server settings
	int         listen_port  = cf->get_int("listen_port");
	std::string forward_host = cf->get_string("forward_host");
	int         forward_port = cf->get_int("forward_port");
	int         max_client   = cf->get_int("max_client");

	log_i("==== startup bruco_proxy ====");
	log_i(
			"config_file=%s, listen_port=%d, forward_host=%s, forward_port=%d, max_client=%d",
			config_file.c_str(), listen_port, forward_host.c_str(), forward_port, max_client);

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

