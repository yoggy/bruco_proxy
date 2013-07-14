#ifndef ____STRING__HPP____
#define ____STRING__HPP____

#include <string>

#ifdef __cplusplus
extern "C" {
#endif 

std::string chomp(const std::string &str);
bool check_comment(const std::string &l);
std::string strip(const std::string &str);
bool split(const std::string &separator, const std::string &src, std::string &key, std::string &val);

#ifdef __cplusplus
};
#endif

#endif // #define ____STRING__HPP____
