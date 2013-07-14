#include "config.hpp"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

Config * Config::singleton_ = NULL;
Mutex Config::singleton_mutex_;

Config * Config::getInstance()
{
	ScopedLock lock(singleton_mutex_);
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

std::string chomp_(const std::string &str)
{
	std::stringstream buf;

	for (unsigned int i = 0; i < str.size(); ++i) {
		if (str[i] == 0x0a || str[i] == 0x0d) continue;
		buf << str[i];
	}

	return buf.str(); 
}

bool check_comment_(const std::string &l)
{
	if (l.size() == 0) return true;
	if (l[0] == '#')   return true;
	if (l.find_first_of("=") == std::string::npos) return true;

	return false;
}

std::string strip_(const std::string &str)
{
	int pos = 0;
	std::string tmp, result;

	if (str.size() == 0) return "";

	// remove whitespace from the head of string.
	for (pos = 0; pos < (int)str.size(); ++pos) {
		if (!(str[pos] == ' ' || str[pos] == '\t')) break;
	}

	if (pos < (int)str.size()) {
		tmp = str.substr(pos);
	}
	else {
		return "";
	}

	// remove whitespace from the end of string.
	for (pos = (int)str.size() - 1; pos > 0; --pos) {
		if (!(str[pos] == ' ' || str[pos] == '\t')) break;
	}

	if (pos >= 0) {
		result = tmp.substr(0, pos+1);
	}
	else {
		return "";
	}

	return result; 
}

bool split_equals_(const std::string &src, std::string &key, std::string &val)
{
	size_t pos = src.find_first_of("=");
	if (pos == std::string::npos) {
		return false;
	}

	key = src.substr(0, pos-1);
	val = src.substr(pos+1);

	key = strip_(key);
	val = strip_(val);

	if (key.size() == 0 || val.size() == 0) {
		return false;
	}

	return true;
}

void Config::set_key_value_(const std::string &l)
{
	std::string key, val;
	if (split_equals_(l, key, val)) {
		map_[key] = val;
	}
}

bool Config::load_(const std::string &filename)
{
	ScopedLock lock(map_mutex_);

	map_.clear();

	std::ifstream ifs;
	ifs.open(filename.c_str(), std::ios::in);
	if (!ifs) {
		return false;
	}

	while(!ifs.eof()) {
		std::string l;
		getline(ifs, l);
		l = chomp_(l);
		if (check_comment_(l)) continue;			
		set_key_value_(l);
	}

	return true;
}
