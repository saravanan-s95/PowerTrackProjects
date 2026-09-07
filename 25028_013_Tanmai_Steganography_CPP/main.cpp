#include <iostream>
#include "Encode.h"
#include "Decode.h"

using namespace std;

/*---------------------------------------------------------------
    Function Name : main()

    Description :
        Entry point of the Steganography application.
        Based on the command line option, it performs
        either Encoding (-e) or Decoding (-d).

    Usage :
        Encoding :
        ./stego -e input/beautiful.bmp input/secret.txt output/stego.bmp

        Decoding :
        ./stego -d output/stego.bmp output/decoded.txt

    Returns :
        0 - Program executed successfully
        1 - Invalid arguments or operation failed
----------------------------------------------------------------*/

int main(int argc, char *argv[])
{
    cout << "=====================================================\n";
    cout << "         IMAGE STEGANOGRAPHY USING C++\n";
    cout << "      Least Significant Bit (LSB) Technique\n";
    cout << "=====================================================\n";

    /* Check whether sufficient command-line arguments are provided */
    if (argc < 2)
    {
        cout << "\n[ERROR] Invalid number of arguments.\n\n";

        cout << "Usage:\n";
        cout << "Encoding : ./stego -e input/beautiful.bmp input/secret.txt output/stego.bmp\n";
        cout << "Decoding : ./stego -d output/stego.bmp output/decoded.txt\n";

        return 1;
    }

    string option = argv[1];

    /*---------------------- Encoding ----------------------*/
    if (option == "-e")
    {
        cout << "\n========== ENCODING SELECTED ==========\n";

        if (argc != 5)
        {
            cout << "[ERROR] Invalid arguments for Encoding.\n";
            cout << "Usage : ./stego -e input/beautiful.bmp input/secret.txt output/stego.bmp\n";
            return 1;
        }

        cout << "[INFO] Initializing Encoder..." << endl;

        Encode encoder(argv[2], argv[3], argv[4]);

        if (encoder.doEncoding() == e_success)
        {
            cout << "\n[SUCCESS] Encoding Completed Successfully.\n";
        }
        else
        {
            cout << "\n[ERROR] Encoding Failed.\n";
        }
    }

    /*---------------------- Decoding ----------------------*/
    else if (option == "-d")
    {
        cout << "\n========== DECODING SELECTED ==========\n";

        if (argc != 4)
        {
            cout << "[ERROR] Invalid arguments for Decoding.\n";
            cout << "Usage : ./stego -d output/stego.bmp output/decoded.txt\n";
            return 1;
        }

        cout << "[INFO] Initializing Decoder..." << endl;

        Decode decoder(argv[2], argv[3]);

        if (decoder.doDecoding() == e_success)
        {
            cout << "\n[SUCCESS] Decoding Completed Successfully.\n";
        }
        else
        {
            cout << "\n[ERROR] Decoding Failed.\n";
        }
    }

    /*---------------- Invalid Option ----------------*/
    else
    {
        cout << "\n[ERROR] Invalid Option.\n";
        cout << "Use -e for Encoding or -d for Decoding.\n";
    }

    return 0;
}