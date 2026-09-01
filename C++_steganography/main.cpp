#include <iostream>
#include <string>

#include "common.h"
#include "Encode.h"
#include "Decode.h"

using namespace std;


int main(int argc, char *argv[])
{
    /*
     * Check command line arguments
     */
    if (argc < 2)
    {
        cout << "ERROR: Invalid arguments" << endl;

        cout << endl;
        cout << "Encoding:" << endl;
        cout << "./a.out -e source.bmp secret.txt output.bmp"
             << endl;

        cout << endl;
        cout << "Decoding:" << endl;
        cout << "./a.out -d stego.bmp output"
             << endl;

        return 0;
    }


    /*
     * Encoding operation
     */
    if (string(argv[1]) == "-e")
    {
        /*
         * Need:
         *
         * argv[0] -> program name
         * argv[1] -> -e
         * argv[2] -> source image
         * argv[3] -> secret file
         * argv[4] -> output image
         */

        if (argc != 5)
        {
            cout << "ERROR: Invalid arguments for encoding"
                 << endl;

            cout << endl;
            cout << "Usage:" << endl;
            cout << "./a.out -e source.bmp secret.txt output.bmp"
                 << endl;

            return 0;
        }


        /*
         * Create Encoder object
         */
        Encoder encoder(
            argv[2],
            argv[3],
            argv[4]
        );


        /*
         * Validate arguments
         */
        if (encoder.validateArguments() != Status::SUCCESS)
        {
            cout << "ERROR: Validation failed"
                 << endl;

            return 0;
        }


        /*
         * Start encoding
         */
        if (encoder.encode() == Status::SUCCESS)
        {
            cout << endl;
            cout << "==============================" << endl;
            cout << "   ENCODING SUCCESSFUL" << endl;
            cout << "==============================" << endl;
        }
        else
        {
            cout << endl;
            cout << "==============================" << endl;
            cout << "   ENCODING FAILED" << endl;
            cout << "==============================" << endl;
        }
    }


    /*
     * Decoding operation
     */
    else if (string(argv[1]) == "-d")
    {
        /*
         * Need:
         *
         * argv[0] -> program name
         * argv[1] -> -d
         * argv[2] -> stego image
         * argv[3] -> output file name
         */

        if (argc != 4)
        {
            cout << "ERROR: Invalid arguments for decoding"
                 << endl;

            cout << endl;
            cout << "Usage:" << endl;
            cout << "./a.out -d stego.bmp output"
                 << endl;

            return 0;
        }


        /*
         * Create Decoder object
         */
        Decoder decoder(
            argv[2],
            argv[3]
        );


        /*
         * Validate arguments
         */
        if (decoder.validateArguments() != Status::SUCCESS)
        {
            cout << "ERROR: Validation failed"
                 << endl;

            return 0;
        }


        /*
         * Start decoding
         */
        if (decoder.decode() == Status::SUCCESS)
        {
            cout << endl;
            cout << "==============================" << endl;
            cout << "   DECODING SUCCESSFUL" << endl;
            cout << "==============================" << endl;
        }
        else
        {
            cout << endl;
            cout << "==============================" << endl;
            cout << "   DECODING FAILED" << endl;
            cout << "==============================" << endl;
        }
    }


    /*
     * Invalid operation
     */
    else
    {
        cout << "ERROR: Unsupported operation" << endl;

        cout << endl;
        cout << "Use:" << endl;
        cout << "-e for encoding" << endl;
        cout << "-d for decoding" << endl;
    }


    return 0;
}

