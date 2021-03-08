#include "SplitString.hpp"

using namespace std;

namespace TFactory {
std::vector<std::string> SplitString::strSplit(const std::string input_str, std::string pattern) {
    std::string::size_type pos;
    std::vector<std::string> result;
    std::string str = input_str;
    str += pattern;
    const int size = str.size();

    for (int i = 0; i < size; i++) {
        pos = str.find(pattern, i);
        if ((int)pos < size) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}
}