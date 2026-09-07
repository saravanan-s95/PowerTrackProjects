#include "Decode.h"
#include <cstring>

using namespace std;

/*---------------------------------------------------------------
Function Name : Decode()

Description :
    Initializes the stego image filename,
    output file name and secret file size.

Arguments :
    stego  - Stego BMP image
    output - Decoded output text file

Returns :
    None
----------------------------------------------------------------*/
Decode::Decode(string stego, string output)
{
    stegoImageName = stego;
    outputFileName = output;

    secretFileSize = 0;
}

/*---------------------------------------------------------------
Function Name : openFiles()

Description :
    Opens the stego BMP image and creates
    the decoded output text file.

Returns :
    e_success - Files opened successfully.
    e_failure - Failed to open files.
----------------------------------------------------------------*/
Status Decode::openFiles()
{
    cout << "\n[INFO] Opening Stego Image..." << endl;

    stegoImage.open(stegoImageName, ios::binary);

    if (!stegoImage)
    {
        cout << "[ERROR] Unable to open Stego Image." << endl;
        return e_failure;
    }

    cout << "[SUCCESS] Stego Image Opened Successfully." << endl;

    cout << "\n[INFO] Creating Output File..." << endl;

    outputFile.open(outputFileName, ios::binary);

    if (!outputFile)
    {
        cout << "[ERROR] Unable to create Output File." << endl;
        return e_failure;
    }

    cout << "[SUCCESS] Output File Created Successfully." << endl;

    return e_success;
}

/*---------------------------------------------------------------
Function Name : decodeByteFromLSB()

Description :
    Decodes one character from the Least
    Significant Bits of 8 image bytes.

Returns :
    Decoded character.
----------------------------------------------------------------*/
char Decode::decodeByteFromLSB(char *buffer)
{
    char ch = 0;

    for (int i = 0; i < 8; i++)
    {
        ch |= (buffer[i] & 1) << (7 - i);
    }

    return ch;
}

/*---------------------------------------------------------------
Function Name : decodeIntFromLSB()

Description :
    Decodes a 32-bit integer from
    32 image bytes.

Returns :
    Decoded integer.
----------------------------------------------------------------*/
int Decode::decodeIntFromLSB(char *buffer)
{
    int value = 0;

    for (int i = 0; i < 32; i++)
    {
        value |= (buffer[i] & 1) << (31 - i);
    }

    return value;
}

/*---------------------------------------------------------------
Function Name : decodeMagicString()

Description :
    Skips the BMP header and verifies
    the encoded magic string.

Returns :
    e_success - Magic string matched.
    e_failure - Magic string mismatch.
----------------------------------------------------------------*/
Status Decode::decodeMagicString()
{
    cout << "\n[INFO] Verifying Magic String..." << endl;

    char buffer[8];
    string magic = "";

    // Skip BMP Header
    stegoImage.seekg(54, ios::beg);

    for (int i = 0; i < strlen(MAGIC_STRING); i++)
    {
        stegoImage.read(buffer, 8);

        magic += decodeByteFromLSB(buffer);
    }

    if (magic == MAGIC_STRING)
    {
        cout << "[SUCCESS] Magic String Matched." << endl;
        return e_success;
    }

    cout << "[ERROR] Magic String Mismatch." << endl;

    return e_failure;
}

/*---------------------------------------------------------------
Function Name : decodeSecretFileSize()

Description :
    Decodes the size of the secret file.

Returns :
    e_success
----------------------------------------------------------------*/
Status Decode::decodeSecretFileSize()
{
    cout << "\n[INFO] Decoding Secret File Size..." << endl;

    char buffer[32];

    stegoImage.read(buffer, 32);

    secretFileSize = decodeIntFromLSB(buffer);

    cout << "[SUCCESS] Secret File Size : "
         << secretFileSize
         << " bytes" << endl;

    return e_success;
}

/*---------------------------------------------------------------
Function Name : decodeSecretData()

Description :
    Decodes the hidden secret data
    from the stego image and writes
    it into the output text file.

Returns :
    e_success
----------------------------------------------------------------*/
Status Decode::decodeSecretData()
{
    cout << "\n[INFO] Decoding Secret Data..." << endl;

    char imageBuffer[8];

    for(int i = 0; i < secretFileSize; i++)
    {
        stegoImage.read(imageBuffer, 8);

        char ch = decodeByteFromLSB(imageBuffer);

        outputFile.put(ch);
    }

    cout << "[SUCCESS] Secret Data Decoded Successfully." << endl;

    return e_success;
}

/*---------------------------------------------------------------
Function Name : doDecoding()

Description :
    Controls the complete decoding
    process.

Returns :
    e_success
    e_failure
----------------------------------------------------------------*/
Status Decode::doDecoding()
{
    cout << "\n========== STARTING DECODING ==========\n";

    if(openFiles() != e_success)
        return e_failure;

    if(decodeMagicString() != e_success)
        return e_failure;

    if(decodeSecretFileSize() != e_success)
        return e_failure;

    if(decodeSecretData() != e_success)
        return e_failure;

    if(stegoImage.is_open())
        stegoImage.close();

    if(outputFile.is_open())
        outputFile.close();

    cout << "\n========== DECODING COMPLETED ==========\n";
    cout << "[SUCCESS] Secret File Recovered Successfully." << endl;

    return e_success;
}