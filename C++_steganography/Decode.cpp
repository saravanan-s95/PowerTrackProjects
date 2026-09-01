#include "Decode.h"
#include "common.h"

using namespace std;


/*
 * Constructor
 */
Decoder::Decoder(string stegoImageName, string secretFileName)
{
    this->stegoImageName = stegoImageName;
    this->secretFileName = secretFileName;

    secretExtensionSize = 0;
    secretFileSize = 0;
}


/*
 * Destructor
 */
Decoder::~Decoder()
{
    if (stegoImage.is_open())
    {
        stegoImage.close();
    }

    if (secretFile.is_open())
    {
        secretFile.close();
    }
}


/*
 * Validate input arguments
 */
Status Decoder::validateArguments()
{
    /*
     * Check whether stego image is BMP
     */
    if (stegoImageName.find(".bmp") == string::npos)
    {
        cout << "ERROR: Stego image must be a .bmp file" << endl;
        return Status::FAILURE;
    }


    /*
     * Check whether stego image exists
     */
    ifstream testFile(stegoImageName, ios::binary);

    if (!testFile)
    {
        cout << "ERROR: Stego image does not exist" << endl;
        return Status::FAILURE;
    }

    testFile.close();


    return Status::SUCCESS;
}


/*
 * Open stego image
 */
Status Decoder::openImageFile()
{
    stegoImage.open(stegoImageName, ios::binary);

    if (!stegoImage)
    {
        cout << "ERROR: Unable to open stego image"
             << endl;

        return Status::FAILURE;
    }


    /*
     * Move after BMP header
     */
    stegoImage.seekg(54, ios::beg);


    return Status::SUCCESS;
}


/*
 * Decode one byte from LSB
 *
 * 8 image bytes contain 1 secret byte.
 */
char Decoder::decodeByteFromLSB(char *buffer)
{
    char data = 0;


    for (int i = 0; i < 8; i++)
    {
        /*
         * Extract LSB
         */
        char bit = buffer[i] & 1;


        /*
         * Shift previous bits to left
         */
        data = (data << 1) | bit;
    }


    return data;
}


/*
 * Decode integer from LSB
 *
 * 32 image bytes contain 1 integer.
 */
int Decoder::decodeIntFromLSB(char *buffer)
{
    int data = 0;


    for (int i = 0; i < 32; i++)
    {
        /*
         * Extract LSB
         */
        int bit = buffer[i] & 1;


        /*
         * Shift and add bit
         */
        data = (data << 1) | bit;
    }


    return data;
}


/*
 * Decode magic string
 */
Status Decoder::decodeMagicString()
{
    string decodedMagic;


    /*
     * Read required number of characters
     */
    for (size_t i = 0; i < string(MAGIC_STRING).length(); i++)
    {
        char buffer[8];


        /*
         * Read 8 image bytes
         */
        stegoImage.read(buffer, 8);


        if (!stegoImage)
        {
            return Status::FAILURE;
        }


        /*
         * Decode one character
         */
        char ch = decodeByteFromLSB(buffer);


        decodedMagic += ch;
    }


    /*
     * Compare decoded magic string
     */
    if (decodedMagic == string(MAGIC_STRING))
    {
        cout << "Magic string matched" << endl;

        return Status::SUCCESS;
    }


    cout << "ERROR: Magic string mismatch" << endl;

    return Status::FAILURE;
}


/*
 * Decode secret file extension size
 */
Status Decoder::decodeSecretFileExtension()
{
    char buffer[32];


    /*
     * Read 32 image bytes
     */
    stegoImage.read(buffer, 32);


    if (!stegoImage)
    {
        return Status::FAILURE;
    }


    /*
     * Decode extension size
     */
    secretExtensionSize =
        static_cast<unsigned int>(
            decodeIntFromLSB(buffer)
        );


    /*
     * Decode extension characters
     */
    secretExtension.clear();


    for (unsigned int i = 0;
         i < secretExtensionSize;
         i++)
    {
        char buffer8[8];


        /*
         * Read 8 image bytes
         */
        stegoImage.read(buffer8, 8);


        if (!stegoImage)
        {
            return Status::FAILURE;
        }


        /*
         * Decode character
         */
        char ch = decodeByteFromLSB(buffer8);


        secretExtension += ch;
    }


    cout << "Secret file extension : "
         << secretExtension << endl;


    return Status::SUCCESS;
}


/*
 * Decode secret file size
 */
Status Decoder::decodeSecretFileSize()
{
    char buffer[32];


    /*
     * Read 32 image bytes
     */
    stegoImage.read(buffer, 32);


    if (!stegoImage)
    {
        return Status::FAILURE;
    }


    /*
     * Decode file size
     */
    secretFileSize =
        static_cast<unsigned long int>(
            decodeIntFromLSB(buffer)
        );


    cout << "Secret file size : "
         << secretFileSize << " bytes"
         << endl;


    return Status::SUCCESS;
}


/*
 * Decode secret file data
 */
Status Decoder::decodeSecretFileData()
{
    /*
     * Create output file name
     *
     * Example:
     *
     * output + ".txt"
     */
    string outputFileName =
        secretFileName + secretExtension;


    /*
     * Open output file
     */
    secretFile.open(outputFileName, ios::binary);


    if (!secretFile)
    {
        cout << "ERROR: Unable to create secret file"
             << endl;

        return Status::FAILURE;
    }


    /*
     * Decode every secret byte
     */
    for (unsigned long int i = 0;
         i < secretFileSize;
         i++)
    {
        char buffer[8];


        /*
         * Read 8 image bytes
         */
        stegoImage.read(buffer, 8);


        if (!stegoImage)
        {
            return Status::FAILURE;
        }


        /*
         * Decode one byte
         */
        char data = decodeByteFromLSB(buffer);


        /*
         * Write decoded byte
         */
        secretFile.write(&data, 1);


        if (!secretFile)
        {
            return Status::FAILURE;
        }
    }


    secretFile.close();


    return Status::SUCCESS;
}


/*
 * Complete decoding operation
 */
Status Decoder::decode()
{
    cout << "Opening stego image" << endl;


    if (openImageFile() != Status::SUCCESS)
    {
        cout << "ERROR: Opening stego image failed"
             << endl;

        return Status::FAILURE;
    }


    cout << "Done opening stego image" << endl;


    /*
     * Decode magic string
     */
    cout << "Decoding Magic String" << endl;


    if (decodeMagicString() != Status::SUCCESS)
    {
        cout << "ERROR: Magic string decoding failed"
             << endl;

        return Status::FAILURE;
    }


    cout << "Done decoding Magic String" << endl;


    /*
     * Decode secret extension
     */
    cout << "Decoding Secret File Extension" << endl;


    if (decodeSecretFileExtension() != Status::SUCCESS)
    {
        cout << "ERROR: Secret file extension decoding failed"
             << endl;

        return Status::FAILURE;
    }


    cout << "Done decoding Secret File Extension"
         << endl;


    /*
     * Decode secret file size
     */
    cout << "Decoding Secret File Size" << endl;


    if (decodeSecretFileSize() != Status::SUCCESS)
    {
        cout << "ERROR: Secret file size decoding failed"
             << endl;

        return Status::FAILURE;
    }


    cout << "Done decoding Secret File Size"
         << endl;


    /*
     * Decode secret file data
     */
    cout << "Decoding Secret File Data" << endl;


    if (decodeSecretFileData() != Status::SUCCESS)
    {
        cout << "ERROR: Secret file data decoding failed"
             << endl;

        return Status::FAILURE;
    }


    cout << "Done decoding Secret File Data"
         << endl;


    cout << "Decoding completed successfully"
         << endl;


    return Status::SUCCESS;
}

