#include "config.hpp"
#include <stdlib.h>

Config * Config::singleton_ = NULL;
Mutex Config::mutex_;

Config * Config::getInstance()
{
	ScopedLock lock(mutex_);
	if (singleton_ == NULL) {
		singleton_ = new Config();
	}
	return singleton_;
}

Config::Config()
{
}

Config::~Config()
{
}


int Config::get_int(const std::string &key)
{
	int val = atoi(get_string(key).c_str());
	return val;
}

std::string Config::get_string(const std::string &key)
{
	if (map_.find(key) == map_.end()) {
		return std::string();
	}
	return map_[key];
}

bool Config::load(const std::string &filename)
{
	Config *cf = getInstance();
	return cf->load_(filename);
}

bool Config::load_(const std::string &filename)
{


	return true;
}

