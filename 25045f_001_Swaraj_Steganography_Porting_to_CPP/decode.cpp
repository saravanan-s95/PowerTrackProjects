#include "decode.h"

//-----------------------------------------------------------
// Constructor
//-----------------------------------------------------------
Decode::Decode(std::string image,
               std::string output)
{
    encodedImageName = image;
    outputFileName = output;
}

//-----------------------------------------------------------
// Open Files
//-----------------------------------------------------------
bool Decode::openFiles()
{
    encodedImage.open(encodedImageName, std::ios::binary);

    outputFile.open(outputFileName);

    if (!encodedImage)
    {
        std::cout << "Error : Unable to open encoded image" << std::endl;
        return false;
    }

    if (!outputFile)
    {
        std::cout << "Error : Unable to create output file" << std::endl;
        return false;
    }

    return true;
}

//-----------------------------------------------------------
// Skip BMP Header
//-----------------------------------------------------------
void Decode::skipBmpHeader()
{
    encodedImage.seekg(54, std::ios::beg);

    std::cout << "BMP Header Skipped Successfully" << std::endl;
}

//-----------------------------------------------------------
// Decode One Character
//-----------------------------------------------------------
char Decode::decodeByteFromLSB()
{
    char imageBuffer[8];

    char data = 0;

    // Read 8 bytes
    encodedImage.read(imageBuffer, 8);

    // Extract LSB
    for (int i = 0; i < 8; i++)
    {
        data = data << 1;

        data = data | (imageBuffer[i] & 1);
    }

    return data;
}

//-----------------------------------------------------------
// Decode Data
//-----------------------------------------------------------
void Decode::decodeDataFromImage(char *data, int size)
{
    for (int i = 0; i < size; i++)
    {
        data[i] = decodeByteFromLSB();
    }
}

///-----------------------------------------------------------
// Decode Magic String
//-----------------------------------------------------------
void Decode::decodeMagicString()
{
    char magic[3];

    decodeDataFromImage(magic, 2);

    magic[2] = '\0';

    if (std::string(magic) == "#*")
    {
        std::cout << "Magic String Verified" << std::endl;
    }
    else
    {
        std::cout << "Error : Magic String Not Found" << std::endl;
    }
}

//-----------------------------------------------------------
// Decode Secret Extension Size
//-----------------------------------------------------------
void Decode::decodeSecretExtensionSize()
{
    extensionSize = (unsigned char)decodeByteFromLSB();

    std::cout << "Extension Size : " << extensionSize << std::endl;
}

//-----------------------------------------------------------
// Decode Secret Extension
//-----------------------------------------------------------
void Decode::decodeSecretExtension()
{
    char extension[20];

    decodeDataFromImage(extension, extensionSize);

    extension[extensionSize] = '\0';

    std::cout << "Extension : " << extension << std::endl;
}

void Decode::decodeSecretFileSize()
{
    char bytes[4];

    decodeDataFromImage(bytes, 4);

    secretFileSize = 0;

    secretFileSize |= ((unsigned char)bytes[0] << 24);
    secretFileSize |= ((unsigned char)bytes[1] << 16);
    secretFileSize |= ((unsigned char)bytes[2] << 8);
    secretFileSize |= (unsigned char)bytes[3];

    std::cout << "Secret File Size : " << secretFileSize << " Bytes" << std::endl;
}

//-----------------------------------------------------------
// Decode Secret Data
//-----------------------------------------------------------
void Decode::decodeSecretData()
{
    char ch;

    for (int i = 0; i < secretFileSize; i++)
    {
        ch = decodeByteFromLSB();

        outputFile.put(ch);
    }

    std::cout << "Secret Data Decoded Successfully" << std::endl;
}

//-----------------------------------------------------------
// Main Decode Function
//-----------------------------------------------------------
void Decode::doDecoding()
{
    std::cout << "\n========== DECODING STARTED ==========\n";

    if (!openFiles())
        return;

    std::cout << "Files Opened Successfully" << std::endl;

    skipBmpHeader();

    decodeMagicString();

    decodeSecretExtensionSize();

    decodeSecretExtension();

    decodeSecretFileSize();

    decodeSecretData();

    std::cout << "\nDecoding Completed Successfully." << std::endl;
}