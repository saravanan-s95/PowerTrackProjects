#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>

const int BMP_HEADER_SIZE = 54;
const std::string MAGIC_STRING = "#*";

enum class Operation
{
    Encode,
    Decode,
    Invalid
};

#endif