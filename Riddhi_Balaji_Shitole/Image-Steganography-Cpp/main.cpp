#include "common.h"
#include "encoder.h"
#include "decoder.h"

int main(int argc, char *argv[])
{
    std::cout << "=====================================\n";
    std::cout << "     Image Steganography in C++\n";
    std::cout << "=====================================\n";

    if (argc < 2)
    {
        std::cout << "Usage:\n";
        std::cout << "./stego -e <input.bmp> <secret.txt> <output.bmp>\n";
        std::cout << "./stego -d <stego.bmp> <output.txt>\n";
        return 1;
    }

    std::string option = argv[1];

    if (option == "-e")
    {
        if (argc != 5)
        {
            std::cout << "Invalid encoding arguments.\n";
            return 1;
        }

        Encoder encoder(argv[2], argv[3], argv[4]);

        if (encoder.encode())
        {
            std::cout << "\nEncoding Successful.\n";
        }
        else
        {
            std::cout << "\nEncoding Failed.\n";
        }
    }
    else if (option == "-d")
{
    if (argc != 4)
    {
        std::cout << "Invalid decoding arguments.\n";
        return 1;
    }

    Decoder decoder(argv[2], argv[3]);

    if (decoder.decode())
        std::cout << "Decoding Successful.\n";
    else
        std::cout << "Decoding Failed.\n";
}
    else
    {
        std::cout << "Invalid option.\n";
    }

    return 0;
}