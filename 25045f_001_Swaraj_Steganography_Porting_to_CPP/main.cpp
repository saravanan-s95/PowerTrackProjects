#include <iostream>
#include "steganography.h"

int main()
{
    Steganography app;

    int choice;

    std::cout << "==================================" << std::endl;
    std::cout << "      STEGANOGRAPHY PROJECT       " << std::endl;
    std::cout << "==================================" << std::endl;

    std::cout << "\n1. Encode" << std::endl;
    std::cout << "2. Decode" << std::endl;

    std::cout << "\nEnter your choice : ";
    std::cin >> choice;

    if (choice == 1)
    {
        app.startEncoding();
    }
    else if (choice == 2)
    {
        app.startDecoding();
    }
    else
    {
        std::cout << "Invalid Choice" << std::endl;
    }

    return 0;
}