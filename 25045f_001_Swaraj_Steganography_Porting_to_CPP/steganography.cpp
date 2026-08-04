#include <iostream>
#include "steganography.h"
#include "decode.h"

// Start encoding process
void Steganography::startEncoding()
{
    std::string srcImage;
    std::string secretFile;
    std::string outputImage;

    // Get source image name
    std::cout << "Enter source BMP image : ";
    std::cin >> srcImage;

    // Get secret file name
    std::cout << "Enter secret file : ";
    std::cin >> secretFile;

    // Get output image name
    std::cout << "Enter output image : ";
    std::cin >> outputImage;

    // Create Encode object
    Encode encoder(srcImage, secretFile, outputImage);

    // Start encoding
    encoder.doEncoding();
}

//-----------------------------------------------------------
// Start Decoding
//-----------------------------------------------------------
void Steganography::startDecoding()
{
    std::string imageFile;
    std::string outputFile;

    std::cout << "Enter encoded BMP image : ";
    std::cin >> imageFile;

    std::cout << "Enter output file name : ";
    std::cin >> outputFile;

    Decode decoder(imageFile, outputFile);

    decoder.doDecoding();
}