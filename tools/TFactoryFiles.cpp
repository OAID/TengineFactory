#include "TFactoryFiles.hpp"

#if defined(_MSC_VER)
#include <Windows.h>
#undef min
#undef max
#else
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#endif

namespace TFactory {
bool TFactoryFiles::file_exist(const char* file) {
#if !defined(_MSC_VER)
    struct stat buffer;
    return stat(file, &buffer) == 0;
#endif
    return true;
}


std::vector<std::string> TFactoryFiles::getFilesName(const char* dir) {
    std::vector<std::string> filesName;
#if defined(_MSC_VER)
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    std::string jpg_files = std::string(dir) + "\\*.jpg"; 
    hFind = FindFirstFile(jpg_files.c_str(), &ffd);
    if (INVALID_HANDLE_VALUE == hFind) {
        std::cout << "open " << dir << " failed: " << strerror(errno) << std::endl;
        return filesName;
    }
    do {
        std::string name = ffd.cFileName;
        std::string file_name = std::string(dir) + "\\" + name;
        if(INVALID_FILE_ATTRIBUTES != GetFileAttributes(file_name.c_str()) && GetLastError() != ERROR_FILE_NOT_FOUND) {
            filesName.push_back(file_name);
        }
    } while (FindNextFile(hFind, &ffd) != 0);
    FindClose(hFind);
#else
    DIR* root;
    if ((root = opendir(dir)) == NULL) {
        std::cout << "open " << dir << " failed: " << strerror(errno) << std::endl;
        return filesName;
    }

    struct dirent* ent;
    while ((ent = readdir(root)) != NULL) {
        if (ent->d_name[0] != '.') {
            std::string filename = std::string(dir) + ent->d_name;
            if (file_exist(filename.c_str())) {
                filesName.push_back(filename);
            }
        }
    }
    closedir(root);
#endif
    return filesName;
}
}