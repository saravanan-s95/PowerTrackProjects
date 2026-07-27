#include "utility.h"

long getFileSize(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);

    if (!file)
        return -1;

    file.seekg(0, std::ios::end);

    return file.tellg();
}