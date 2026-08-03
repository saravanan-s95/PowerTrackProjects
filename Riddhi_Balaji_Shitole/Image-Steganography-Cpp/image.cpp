#include "image.h"

bool Image::openInput(const std::string &filename)
{
    inputFile.open(filename, std::ios::binary);

    if (!inputFile)
    {
        std::cout << "Error opening input image\n";
        return false;
    }

    return true;
}

bool Image::openOutput(const std::string &filename)
{
    outputFile.open(filename, std::ios::binary);

    if (!outputFile)
    {
        std::cout << "Error creating output image\n";
        return false;
    }

    return true;
}
bool Image::encodeByte(char data)
{
    char imageByte;

    for (int i = 7; i >= 0; i--)
    {
        if (!inputFile.read(&imageByte, 1))
            return false;

        int bit = (data >> i) & 1;

        imageByte = (imageByte & 0xFE) | bit;

        outputFile.write(&imageByte, 1);
    }

    return true;
}

void Image::copyHeader()
{
    char header[54];

    inputFile.read(header, 54);

    outputFile.write(header, 54);
}

char Image::readByte()
{
    char ch;
    inputFile.read(&ch, 1);
    return ch;
}

void Image::writeByte(char ch)
{
    outputFile.write(&ch, 1);
}

void Image::copyRemaining()
{
    char ch;

    while (inputFile.read(&ch, 1))
    {
        outputFile.write(&ch, 1);
    }
}

void Image::close()
{
    if (inputFile.is_open())
        inputFile.close();

    if (outputFile.is_open())
        outputFile.close();
}

long Image::getImageSize()
{
    inputFile.seekg(0, std::ios::end);

    long size = inputFile.tellg();

    inputFile.seekg(54, std::ios::beg);

    return size - 54;
}