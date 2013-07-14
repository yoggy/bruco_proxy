#ifndef ____CONFIG__HPP____
#define ____CONFIG__HPP____

#include <string>
#include <map>
#include "lock.hpp"

class Config
{
	private:
		Config();
		virtual ~Config();

	public:
		int get_int(const std::string &key);
		std::string get_string(const std::string &key);

	public:
		static Config *getInstance();
		static bool load(const std::string &filename);

	private:
		bool load_(const std::string &filename);

	private:
		std::map<std::string, std::string> map_;
		
	private:
		static Config *singleton_;
		static Mutex   mutex_;
};



#endif // #define ____CONFIG__HPP____