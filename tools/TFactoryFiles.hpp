#include <iostream>
#include <vector>
#include <string.h>
#include <cstring>

namespace TFactory {
class TFactoryFiles
{
public:
    static bool file_exist(const char* file);
    static std::vector<std::string> getFilesName(const char* dir);
};
}