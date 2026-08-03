#include "decoder.h"

Decoder::Decoder(const std::string &input,
                 const std::string &output)
{
    inputImage = input;
    outputFile = output;
}

bool Decoder::openFiles()
{
    imageFile.open(inputImage, std::ios::binary);

    if (!imageFile)
    {
        std::cout << "Unable to open stego image.\n";
        return false;
    }

    secretOutput.open(outputFile, std::ios::binary);

    if (!secretOutput)
    {
        std::cout << "Unable to create output file.\n";
        return false;
    }

    return true;
}

char Decoder::decodeByte()
{
    char imageByte;
    char data = 0;

    for (int i = 7; i >= 0; i--)
    {
        imageFile.read(&imageByte, 1);

        data |= ((imageByte & 1) << i);
    }

    return data;
}

int Decoder::decodeInt()
{
    int value = 0;

    for (int i = 3; i >= 0; i--)
    {
        char byte = decodeByte();

        value |= ((unsigned char)byte << (i * 8));
    }

    return value;
}
bool Decoder::decode()
{
    std::cout << "\n===== Decoding Started =====\n";

    if (!openFiles())
        return false;

    // Skip the 54-byte BMP header
    imageFile.seekg(54, std::ios::beg);

   if (!decodeMagicString())
    return false;

   if (!decodeExtension())
    return false;

   if (!decodeFileSize())
    return false;

   if (!decodeSecretFile())
    return false;

   std::cout << "Decoding Completed.\n";

    imageFile.close();
    secretOutput.close();

    return true;
}
std::string Decoder::decodeString(int length)
{
    std::string str;

    for (int i = 0; i < length; i++)
    {
        str += decodeByte();
    }

    return str;
}bool Decoder::decodeMagicString()
{
    std::cout << "Decoding Magic String...\n";

    std::string magic = decodeString(2);

    if (magic != "#*")
    {
        std::cout << "Magic String Mismatch!\n";
        return false;
    }

    std::cout << "Magic String Verified.\n";

    return true;
}
bool Decoder::decodeExtension()
{
    std::cout << "Decoding File Extension...\n";

    int length = decodeInt();

    std::string extension = decodeString(length);

    std::cout << "Extension : " << extension << std::endl;

    return true;
}
bool Decoder::decodeFileSize()
{
    std::cout << "Decoding Secret File Size...\n";

    secretFileSize = decodeInt();

    std::cout << "Secret File Size : "
              << secretFileSize << " bytes\n";

    return true;
}
bool Decoder::decodeSecretFile()
{
    std::cout << "Decoding Secret File...\n";

    for (int i = 0; i < secretFileSize; i++)
    {
        char ch = decodeByte();
        secretOutput.put(ch);
    }

    std::cout << "Secret File Extracted Successfully.\n";

    return true;
}