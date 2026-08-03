#ifndef IMAGE_H
#define IMAGE_H

#include "common.h"

class Image
{
private:
    std::ifstream inputFile;
    std::ofstream outputFile;

public:
    bool openInput(const std::string &filename);
    bool openOutput(const std::string &filename);

    void copyHeader();

    char readByte();

    void writeByte(char ch);

    bool encodeByte(char data);

    void copyRemaining();

    void close();
    
    long getImageSize();
};

#endif