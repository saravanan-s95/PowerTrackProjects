#include "Encode.h"
#include <cstring>

using namespace std;

/*---------------------------------------------------------------
Function Name : Encode()

Description :
    Initializes the source image filename,
    secret file filename, output stego image filename,
    image capacity and secret file size.

Arguments :
    src    - Source BMP image
    secret - Secret text file
    stego  - Output stego image

Returns :
    None
----------------------------------------------------------------*/
Encode::Encode(string src, string secret, string stego)
{
    srcImageName = src;
    secretFileName = secret;
    stegoImageName = stego;

    imageCapacity = 0;
    secretFileSize = 0;
}
/*---------------------------------------------------------------
Function Name : openFiles()

Description :
    Opens the source BMP image, secret file,
    and creates the output stego image.

Returns :
    e_success : Files opened successfully.
    e_failure : Failed to open one or more files.
----------------------------------------------------------------*/

Status Encode::openFiles()
{
    cout << "\n[INFO] Opening Source Image..." << endl;

    srcImage.open(srcImageName, ios::binary);

    if (!srcImage)
    {
        cout << "[ERROR] Unable to open Source Image." << endl;
        return e_failure;
    }

    cout << "[SUCCESS] Source Image Opened Successfully." << endl;


    cout << "\n[INFO] Opening Secret File..." << endl;

    secretFile.open(secretFileName, ios::binary);

    if (!secretFile)
    {
        cout << "[ERROR] Unable to open Secret File." << endl;
        return e_failure;
    }

    cout << "[SUCCESS] Secret File Opened Successfully." << endl;


    cout << "\n[INFO] Creating Stego Image..." << endl;

    stegoImage.open(stegoImageName, ios::binary);

    if (!stegoImage)
    {
        cout << "[ERROR] Unable to create Stego Image." << endl;
        return e_failure;
    }

    cout << "[SUCCESS] Stego Image Created Successfully." << endl;

    return e_success;
}
/*---------------------------------------------------------------
Function Name : checkCapacity()

Description :
    Calculates the image capacity and verifies
    whether the image has enough space to
    store the secret file.

Returns :
    e_success
    e_failure
----------------------------------------------------------------*/

Status Encode::checkCapacity()
{
    cout << "\n[INFO] Checking Image Capacity..." << endl;

    srcImage.seekg(0, ios::end);

    long imageSize = srcImage.tellg();

    srcImage.seekg(54, ios::beg);

    secretFile.seekg(0, ios::end);

    secretFileSize = secretFile.tellg();

    secretFile.seekg(0, ios::beg);

    imageCapacity = imageSize - 54;

    if(secretFileSize == 0)
    {
        cout << "[ERROR] Secret File is Empty." << endl;
        return e_failure;
    }

    if(imageCapacity >= (secretFileSize * 8 + 100))
    {
        cout << "[SUCCESS] Image has Enough Capacity." << endl;
        return e_success;
    }

    cout << "[ERROR] Image does not have Enough Capacity." << endl;

    return e_failure;
}
/* Encode one character into 8 bytes using LSB */
void Encode::encodeByteToLSB(char data, char *buffer)
{
    for (int i = 0; i < 8; i++)
    {
        buffer[i] = (buffer[i] & 0xFE) | ((data >> (7 - i)) & 1);
    }
}
/* Encode integer into 32 bytes */
void Encode::encodeIntToLSB(int data, char *buffer)
{
    for (int i = 0; i < 32; i++)
    {
        buffer[i] = (buffer[i] & 0xFE) | ((data >> (31 - i)) & 1);
    }
}
/*---------------------------------------------------------------
Function Name : copyBMPHeader()

Description :
    Copies the first 54 bytes (BMP Header)
    from source image to stego image.

Returns :
    e_success
----------------------------------------------------------------*/

Status Encode::copyBMPHeader()
{
    cout << "\n[INFO] Copying BMP Header..." << endl;

    char header[54];

    srcImage.seekg(0);

    srcImage.read(header,54);

    stegoImage.write(header,54);

    cout << "[SUCCESS] BMP Header Copied Successfully." << endl;

    return e_success;
}
/*---------------------------------------------------------------
Function Name : encodeMagicString()

Description :
    Encodes the predefined magic string "#*"
    into the source image.

Returns :
    e_success
----------------------------------------------------------------*/
Status Encode::encodeMagicString()
{
    char buffer[8];
    cout << "\n[INFO] Encoding Magic String..." << endl;

    for (int i = 0; i < strlen(MAGIC_STRING); i++)
    {
        srcImage.read(buffer, 8);

        encodeByteToLSB(MAGIC_STRING[i], buffer);

        stegoImage.write(buffer, 8);
    }
    cout << "[SUCCESS] Magic String Encoded Successfully." << endl;
    return e_success;
}
Status Encode::encodeSecretFileSize()
{
    char buffer[32];

    cout << "\n[INFO] Encoding Secret File Size..." << endl;

    srcImage.read(buffer, 32);

    encodeIntToLSB(secretFileSize, buffer);

    stegoImage.write(buffer, 32);

    cout << "[SUCCESS] Secret File Size Encoded Successfully." << endl;

    return e_success;
}
Status Encode::encodeSecretData()
{
    char imageBuffer[8];
    char ch;
    cout << "\n[INFO] Encoding Secret Data..." << endl;

    while (secretFile.get(ch))
    {
        srcImage.read(imageBuffer, 8);

        encodeByteToLSB(ch, imageBuffer);

        stegoImage.write(imageBuffer, 8);
    }
    cout << "[SUCCESS] Secret Data Encoded Successfully." << endl;
    return e_success;
}
Status Encode::copyRemainingData()
{
    char ch;

    cout << "\n[INFO] Copying Remaining Image Data..." << endl;

    while (srcImage.get(ch))
    {
        stegoImage.put(ch);
    }
    cout << "[SUCCESS] Remaining Image Data Copied." << endl;

    return e_success;
}
/*---------------------------------------------------------------
Function Name : doEncoding()

Description :
    Performs the complete encoding process.

Returns :
    e_success
    e_failure
----------------------------------------------------------------*/

Status Encode::doEncoding()
{
    cout << "\n========== STARTING ENCODING ==========\n";

    if(openFiles()!=e_success)
        return e_failure;

    if(checkCapacity()!=e_success)
        return e_failure;

    if(copyBMPHeader()!=e_success)
        return e_failure;

    if(encodeMagicString()!=e_success)
        return e_failure;

    if(encodeSecretFileSize()!=e_success)
        return e_failure;

    if(encodeSecretData()!=e_success)
        return e_failure;

    if(copyRemainingData()!=e_success)
        return e_failure;

    if(srcImage.is_open())
        srcImage.close();

    if(secretFile.is_open())
        secretFile.close();

    if(stegoImage.is_open())
        stegoImage.close();

    cout << "\n========== ENCODING COMPLETED ==========\n";
    cout << "[SUCCESS] Stego Image Created Successfully." << endl;

    return e_success;
}
