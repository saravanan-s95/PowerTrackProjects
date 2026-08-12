#include "encode.h"

//-----------------------------------------------------------
// Constructor
//-----------------------------------------------------------
Encode::Encode(std::string src, std::string secret, std::string output)
{
    srcImageName = src;
    secretFileName = secret;
    outputImageName = output;
}

//-----------------------------------------------------------
// Validate file names
//-----------------------------------------------------------
bool Encode::validateFiles()
{
    // Check source image extension
    size_t pos = srcImageName.find_last_of('.');

    if (pos == std::string::npos)
    {
        std::cout << "Error : Source image has no extension" << std::endl;
        return false;
    }

    if (srcImageName.substr(pos) != ".bmp")
    {
        std::cout << "Error : Source image must be BMP file" << std::endl;
        return false;
    }

    // Check secret file extension
    pos = secretFileName.find_last_of('.');

    if (pos == std::string::npos)
    {
        std::cout << "Error : Secret file has no extension" << std::endl;
        return false;
    }

    return true;
}

//-----------------------------------------------------------
// Open all files
//-----------------------------------------------------------
bool Encode::openFiles()
{
    srcImage.open(srcImageName, std::ios::binary);

    secretFile.open(secretFileName, std::ios::binary);

    outputImage.open(outputImageName, std::ios::binary);

    if (!srcImage)
    {
        std::cout << "Error : Unable to open source image" << std::endl;
        return false;
    }

    if (!secretFile)
    {
        std::cout << "Error : Unable to open secret file" << std::endl;
        return false;
    }

    if (!outputImage)
    {
        std::cout << "Error : Unable to create output image" << std::endl;
        return false;
    }

    return true;
}

//-----------------------------------------------------------
// Check image capacity
//-----------------------------------------------------------
bool Encode::checkCapacity()
{
    // Get image size
    srcImage.seekg(0, std::ios::end);
    long imageSize = srcImage.tellg();
    srcImage.seekg(0, std::ios::beg);

    // Get secret file size
    secretFile.seekg(0, std::ios::end);
    long secretSize = secretFile.tellg();
    secretFile.seekg(0, std::ios::beg);

    long requiredSize = secretSize * 8;

    std::cout << "\nImage Size    : " << imageSize << " Bytes" << std::endl;
    std::cout << "Secret Size   : " << secretSize << " Bytes" << std::endl;
    std::cout << "Required Size : " << requiredSize << " Bytes" << std::endl;

    if (imageSize < requiredSize)
    {
        std::cout << "Error : Image does not have enough capacity" << std::endl;
        return false;
    }

    return true;
}

//-----------------------------------------------------------
// Copy BMP Header (54 Bytes)
//-----------------------------------------------------------
void Encode::copyBmpHeader()
{
    char header[54];

    srcImage.read(header, 54);

    outputImage.write(header, 54);

    std::cout << "BMP Header Copied Successfully" << std::endl;
}

//-----------------------------------------------------------
// Encode one character using LSB
//-----------------------------------------------------------
void Encode::encodeByteToLSB(char data)
{
    // Store 8 bytes from image
    char imageBuffer[8];

    // Read 8 bytes from source image
    srcImage.read(imageBuffer, 8);

    // Replace LSB of every image byte
    for (int i = 0; i < 8; i++)
    {
        // Clear last bit
        imageBuffer[i] = imageBuffer[i] & 0xFE;

        // Get one bit from character
        int bit = (data >> (7 - i)) & 1;

        // Put that bit into image byte
        imageBuffer[i] = imageBuffer[i] | bit;
    }

    // Write modified bytes to output image
    outputImage.write(imageBuffer, 8);
}

//-----------------------------------------------------------
// Encode complete data into image
//-----------------------------------------------------------
void Encode::encodeDataToImage(const char *data, int size)
{
    // Encode every character
    for (int i = 0; i < size; i++)
    {
        encodeByteToLSB(data[i]);
    }
}

//-----------------------------------------------------------
// Encode magic string
//-----------------------------------------------------------
void Encode::encodeMagicString()
{
    std::string magic = "#*";

    encodeDataToImage(magic.c_str(), magic.length());

    std::cout << "Magic String Encoded Successfully" << std::endl;
}

//-----------------------------------------------------------
// Encode secret file extension size
//-----------------------------------------------------------
void Encode::encodeSecretExtensionSize()
{
    // Find last dot
    size_t pos = secretFileName.find_last_of('.');

    // Get extension
    std::string extension = secretFileName.substr(pos);

    int size = extension.length();

    char ch = (char)size;

    encodeByteToLSB(ch);

    std::cout << "Extension Size Encoded Successfully" << std::endl;
}

//-----------------------------------------------------------
// Encode secret file extension
//-----------------------------------------------------------
void Encode::encodeSecretExtension()
{
    size_t pos = secretFileName.find_last_of('.');

    std::string extension = secretFileName.substr(pos);

    encodeDataToImage(extension.c_str(), extension.length());

    std::cout << "Extension Encoded Successfully" << std::endl;
}

//-----------------------------------------------------------
// Encode secret file size
//-----------------------------------------------------------
void Encode::encodeSecretFileSize()
{
    // Move pointer to end
    secretFile.seekg(0, std::ios::end);

    // Get file size
    int size = secretFile.tellg();

    // Move pointer back
    secretFile.seekg(0, std::ios::beg);

    // Store size in 4 bytes
    char bytes[4];

    bytes[0] = (size >> 24) & 0xFF;
    bytes[1] = (size >> 16) & 0xFF;
    bytes[2] = (size >> 8) & 0xFF;
    bytes[3] = size & 0xFF;

    // Encode all 4 bytes
    encodeDataToImage(bytes, 4);

    std::cout << "Secret File Size Encoded Successfully" << std::endl;
}

//-----------------------------------------------------------
// Encode secret file data
//-----------------------------------------------------------
void Encode::encodeSecretData()
{
    char ch;

    while (secretFile.get(ch))
    {
        encodeByteToLSB(ch);
    }

    std::cout << "Secret Data Encoded Successfully" << std::endl;
}

//-----------------------------------------------------------
// Copy remaining image data
//-----------------------------------------------------------
void Encode::copyRemainingImageData()
{
    char ch;

    while (srcImage.get(ch))
    {
        outputImage.put(ch);
    }

    std::cout << "Remaining Image Copied Successfully" << std::endl;
}

//-----------------------------------------------------------
// Main Encoding Function
//-----------------------------------------------------------
void Encode::doEncoding()
{
    std::cout << "\n========== ENCODING STARTED ==========\n";

    if (!validateFiles())
        return;

    if (!openFiles())
        return;

    std::cout << "Files Opened Successfully" << std::endl;

    if (!checkCapacity())
        return;

    std::cout << "Capacity Check Successful" << std::endl;

    copyBmpHeader();

    encodeMagicString();

    encodeSecretExtensionSize();

    encodeSecretExtension();

    encodeSecretFileSize();

    encodeSecretData();

    copyRemainingImageData();

    std::cout << "\nEncoding Completed Successfully." << std::endl;

}