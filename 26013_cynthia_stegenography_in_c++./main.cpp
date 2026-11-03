
#include <iostream>
#include <cstring>
#include "encode.h"
#include "decode.h"

using namespace std;

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    if (argc != 4)
    {
        cout << "Invalid arguments" << endl;
        return 1;
    }

    if (strcmp(argv[1], "-e") == 0)
    {
        if (read_and_validate_encode_args(argv, &encInfo) == e_success)
        {
            if (do_encoding(&encInfo) == e_success)
            {
                cout << "Encoding successful" << endl;
            }
            else
            {
                cout << "Encoding failed" << endl;
            }
        }
        else
        {
            cout << "Invalid input files" << endl;
        }
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        if (read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            if (do_decoding(&decInfo) == e_success)
            {
                cout << "Decoding successful" << endl;
            }
            else
            {
                cout << "Decoding failed" << endl;
            }
        }
        else
        {
            cout << "Invalid input files" << endl;
        }
    }
    else
    {
        cout << "Invalid operation" << endl;
    }

    return 0;
}
