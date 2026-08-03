#include "encoder.h"
#include "utility.h"

Encoder::Encoder(const std::string &input,
                 const std::string &secret,
                 const std::string &output)
{
    inputImage = input;
    secretFile = secret;
    outputImage = output;
}

bool Encoder::openFiles()
{
    if (!image.openInput(inputImage))
        return false;

    if (!image.openOutput(outputImage))
        return false;

    secretInput.open(secretFile, std::ios::binary);

    if (!secretInput)
    {
        std::cout << "Unable to open secret file.\n";
        return false;
    }

    return true;
}
bool Encoder::checkCapacity()
{
    imageCapacity = image.getImageSize();
    secretFileSize = getFileSize(secretFile);

    if (secretFileSize == -1)
    {
        std::cout << "Unable to open secret file\n";
        return false;
    }

    long required = (2 * 8) + (secretFileSize * 8);

    std::cout << "Image Capacity  : " << imageCapacity << " bytes\n";
    std::cout << "Secret File Size: " << secretFileSize << " bytes\n";

    if (imageCapacity < required)
    {
        std::cout << "Image does not have enough capacity.\n";
        return false;
    }

    std::cout << "Image has sufficient capacity.\n";
    return true;
}

bool Encoder::encodeMagicString()
{
    std::cout << "Encoding Magic String...\n";

    std::string magic = "#*";

    for (char ch : magic)
    {
        if (!image.encodeByte(ch))
            return false;
    }

    return true;
}

bool Encoder::encodeSecretFile()
{
    std::cout << "Encoding Secret File...\n";

    char ch;

    while (secretInput.get(ch))
    {
        if (!image.encodeByte(ch))
            return false;
    }

    return true;
}

bool Encoder::encode()
{
    std::cout << "\n===== Encoding Started =====\n";

    if (!openFiles())
        return false;

    image.copyHeader();

    if (!checkCapacity())
        return false;

    if (!encodeMagicString())
        return false;

    if (!encodeFileExtension())
    return false;

    if (!encodeFileSize())
    return false;

    if (!encodeSecretFile())
    return false;

    image.copyRemaining();
    image.close();

    std::cout << "Encoding Completed.\n";

    return true;
}
bool Encoder::encodeString(const std::string &str)
{
    for(char ch : str)
    {
        if(!image.encodeByte(ch))
            return false;
    }

    return true;
}
bool Encoder::encodeInt(int value)
{
    for(int i = 3; i >= 0; i--)
    {
        char byte = (value >> (i * 8)) & 0xFF;

        if(!image.encodeByte(byte))
            return false;
    }

    return true;
}
bool Encoder::encodeFileExtension()
{
    std::cout << "Encoding File Extension...\n";

    std::string extension = ".txt";

    if(!encodeInt(extension.length()))
        return false;

    if(!encodeString(extension))
        return false;

    return true;
}
bool Encoder::encodeFileSize()
{
    std::cout << "Encoding Secret File Size...\n";

    if (!encodeInt(secretFileSize))
        return false;

    return true;
}