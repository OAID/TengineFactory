#ifndef SPLITSTRING_HPP
#define SPLITSTRING_HPP
#include <vector>
#include <string>

namespace TFactory {
class SplitString
{
public:
    static std::vector<std::string> strSplit(const std::string input_str, std::string pattern);
};
}

#endif