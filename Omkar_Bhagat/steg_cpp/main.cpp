#include <iostream>
#include <cstring>

#include "Encode.hpp"
#include "Decode.hpp"

using namespace std;

/* Function Prototype */
OperationType checkOperationType(char *argv[]);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "ERROR: No operation specified" << endl;

        cout << "Usage :" << endl;
        cout << "./a.out -e <input.bmp> <secret.txt> <stego.bmp>" << endl;
        cout << "./a.out -d <stego.bmp> <output.txt>" << endl;

        return 1;
    }

    OperationType choice =
        checkOperationType(argv);

    if (choice == e_encode)
    {
        cout << "Encoding Selected"
             << endl;

        Encode enc;

        if (enc.readAndValidateArgs(argv)
            == e_success)
        {
            cout << "Arguments Validated"
                 << endl;

            if (enc.doEncoding()
                == e_success)
            {
                cout << "Encoding Completed Successfully"
                     << endl;
            }
            else
            {
                cout << "Encoding Failed"
                     << endl;
            }
        }
        else
        {
            cout << "Invalid Encode Arguments"
                 << endl;

            return 1;
        }
    }
    else if (choice == e_decode)
    {
        cout << "Decoding Selected"
             << endl;

        Decode dec;

        if (dec.readAndValidateArgs(argv)
            == e_success)
        {
            cout << "Arguments Validated"
                 << endl;

            if (dec.doDecoding()
                == e_success)
            {
                cout << "Decoding Completed Successfully"
                     << endl;
            }
            else
            {
                cout << "Decoding Failed"
                     << endl;
            }
        }
        else
        {
            cout << "Invalid Decode Arguments"
                 << endl;

            return 1;
        }
    }
    else
    {
        cout << "Invalid Option" << endl;

        cout << "Encoding :" << endl;
        cout << "./a.out -e beautiful.bmp secret.txt stego.bmp"
             << endl;

        cout << "Decoding :" << endl;
        cout << "./a.out -d stego.bmp output.txt"
             << endl;

        return 1;
    }

    return 0;
}

/* Check Operation Type */
OperationType checkOperationType(char *argv[])
{
    if (strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}