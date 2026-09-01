#include "Encode.h"
#include "common.h"

using namespace std;


/*
 * Constructor
 */
Encoder::Encoder(string sourceImage, string secretFileName, string outputImage)
{
    srcImageName = sourceImage;
    this->secretFileName = secretFileName;
    outputImageName = outputImage;

    secretExtensionSize = 0;
    secretFileSize = 0;
}


/*
 * Destructor
 */
Encoder::~Encoder()
{
    if (srcImage.is_open())
        srcImage.close();

    if (secretFile.is_open())
        secretFile.close();

    if (outputImage.is_open())
        outputImage.close();
}


/*
 * Validate input arguments
 */
Status Encoder::validateArguments()
{
    /*
     * Check source image extension
     */
    size_t pos = srcImageName.find_last_of('.');

    if (pos == string::npos ||srcImageName.substr(pos) != ".bmp")
    {
        cout << "ERROR: Source image must be .bmp" << endl;
        return Status::FAILURE;
    }


    /*
     * Check whether source image exists
     */
    ifstream testSource(srcImageName, ios::binary);

    if (!testSource)
    {
        cout << "ERROR: Source image does not exist" << endl;
        return Status::FAILURE;
    }

    testSource.close();


    /*
     * Get secret file extension
     */
    
    pos = secretFileName.find_last_of('.');

    if (pos == string::npos)
    {
        cout << "ERROR: Secret file must have an extension"<< endl;

        return Status::FAILURE;
    }


    /*
     * Store extension
     */
    secretExtension = secretFileName.substr(pos);


    /*
     * Check supported secret extensions
     */
    if (secretExtension != ".txt" &&secretExtension != ".c" &&secretExtension != ".cpp")
    {
        cout << "ERROR: Unsupported secret file extension: "<< secretExtension << endl;

        return Status::FAILURE;
    }


    /*
     * Check whether secret file exists
     */
    ifstream testSecret(secretFileName, ios::binary);

    if (!testSecret)
    {
        cout << "ERROR: Secret file does not exist"
             << endl;

        return Status::FAILURE;
    }

    testSecret.close();


    /*
     * Check output image extension
     */
    pos = outputImageName.find_last_of('.');

    if (pos == string::npos ||outputImageName.substr(pos) != ".bmp")
    {
        cout << "ERROR: Output image must be .bmp"
             << endl;

        return Status::FAILURE;
    }


    return Status::SUCCESS;
}

/*
 * Open source image, secret file and output image
 */
Status Encoder::openFiles()
{
    srcImage.open(srcImageName, ios::binary);

    if (!srcImage)
    {
        cout << "ERROR: Unable to open source image "
             << srcImageName << endl;

        return Status::FAILURE;
    }


    secretFile.open(secretFileName, ios::binary);

    if (!secretFile)
    {
        cout << "ERROR: Unable to open secret file "
             << secretFileName << endl;

        return Status::FAILURE;
    }


    outputImage.open(outputImageName, ios::binary);

    if (!outputImage)
    {
        cout << "ERROR: Unable to create output image "
             << outputImageName << endl;

        return Status::FAILURE;
    }


    return Status::SUCCESS;
}


/*
 * Get image size for BMP
 *
 * BMP width  -> offset 18
 * BMP height -> offset 22
 *
 * Capacity = width * height * 3
 */
unsigned int Encoder::getImageSize()
{
    unsigned int width = 0;
    unsigned int height = 0;


    /*
     * Move to byte 18
     */
    srcImage.seekg(18, ios::beg);


    /*
     * Read width
     */
    srcImage.read(reinterpret_cast<char *>(&width),
                  sizeof(width));


    /*
     * Read height
     */
    srcImage.read(reinterpret_cast<char *>(&height),
                  sizeof(height));


    return width * height * 3;
}


/*
 * Get secret file size
 */
unsigned int Encoder::getSecretFileSize()
{
    /*
     * Move to end of file
     */
    secretFile.seekg(0, ios::end);


    unsigned int size =
        static_cast<unsigned int>(secretFile.tellg());


    /*
     * Move back to beginning
     */
    secretFile.seekg(0, ios::beg);


    return size;
}


/*
 * Check whether image has enough capacity
 */
Status Encoder::checkCapacity()
{
    unsigned int imageCapacity = getImageSize();

    secretFileSize = getSecretFileSize();

    unsigned long long int encodingThings =(string(MAGIC_STRING).length()+ 4+ secretExtension.length()+ 4+ secretFileSize) * 8;

    if (imageCapacity > encodingThings)
    {
        return Status::SUCCESS;
    }
    else
    {
        return Status::FAILURE;
    }
}


/*
 * Copy BMP header
 *
 * First 54 bytes are copied without modification.
 */
Status Encoder::copyBmpHeader()
{
    char header[54];


    /*
     * Move source image to beginning
     */
    srcImage.seekg(0, ios::beg);


    /*
     * Read 54 bytes
     */
    srcImage.read(header, 54);


    if (!srcImage)
    {
        return Status::FAILURE;
    }


    /*
     * Write 54 bytes to output
     */
    outputImage.write(header, 54);


    if (!outputImage)
    {
        return Status::FAILURE;
    }


    return Status::SUCCESS;
}


/*
 * Encode one byte into 8 image bytes
 *
 * Example:
 *
 * Secret byte = 01001001
 *
 * Image bytes:
 * xxxxxxx0
 * xxxxxxx1
 * xxxxxxx0
 * xxxxxxx0
 * xxxxxxx1
 * xxxxxxx0
 * xxxxxxx0
 * xxxxxxx1
 *
 * Only LSB is changed.
 */
Status Encoder::encodeByteToLSB(char data, char *buffer)
{
    for (int i = 0; i < 8; i++)
    {
        /*
         * Clear LSB
         */
        buffer[i] = buffer[i] & ~(1 << 0);


        /*
         * Extract bit from secret data
         */
        int bit = (data >> (7 - i)) & 1;


        /*
         * Put bit into image byte LSB
         */
        buffer[i] = buffer[i] | bit;
    }


    return Status::SUCCESS;
}


/*
 * Encode integer into 32 image bytes
 */
Status Encoder::encodeIntToLSB(int data, char *buffer)
{
    for (int i = 0; i < 32; i++)
    {
        /*
         * Clear LSB
         */
        buffer[i] = buffer[i] & ~(1 << 0);


        /*
         * Extract bit
         */
        int bit = (data >> (31 - i)) & 1;


        /*
         * Store bit in LSB
         */
        buffer[i] = buffer[i] | bit;
    }


    return Status::SUCCESS;
}


/*
 * Encode magic string
 */
Status Encoder::encodeMagicString(const string &magicString)
{
    for (size_t i = 0; i < magicString.length(); i++)
    {
        char buffer[8];


        /*
         * Read 8 image bytes
         */
        srcImage.read(buffer, 8);


        if (!srcImage)
        {
            return Status::FAILURE;
        }


        /*
         * Encode one character
         */
        encodeByteToLSB(magicString[i], buffer);


        /*
         * Write modified image bytes
         */
        outputImage.write(buffer, 8);


        if (!outputImage)
        {
            return Status::FAILURE;
        }
    }


    return Status::SUCCESS;
}


/*
 * Encode secret file extension
 */
Status Encoder::encodeSecretFileExtension()
{
    /*
     * Store extension size
     */
    secretExtensionSize =
        static_cast<unsigned int>(secretExtension.length());


    char buffer32[32];


    /*
     * Read 32 image bytes
     */
    srcImage.read(buffer32, 32);


    if (!srcImage)
    {
        return Status::FAILURE;
    }


    /*
     * Encode extension length
     */
    encodeIntToLSB(secretExtensionSize, buffer32);


    /*
     * Write modified bytes
     */
    outputImage.write(buffer32, 32);


    /*
     * Encode extension characters
     */
    for (unsigned int i = 0;
         i < secretExtensionSize;
         i++)
    {
        char buffer8[8];


        /*
         * Read 8 image bytes
         */
        srcImage.read(buffer8, 8);


        if (!srcImage)
        {
            return Status::FAILURE;
        }


        /*
         * Encode extension character
         */
        encodeByteToLSB(secretExtension[i], buffer8);


        /*
         * Write modified bytes
         */
        outputImage.write(buffer8, 8);
    }


    return Status::SUCCESS;
}


/*
 * Encode secret file size
 */
Status Encoder::encodeSecretFileSize()
{
    char buffer32[32];


    /*
     * Read 32 image bytes
     */
    srcImage.read(buffer32, 32);


    if (!srcImage)
    {
        return Status::FAILURE;
    }


    /*
     * Encode secret file size
     */
    encodeIntToLSB(
        static_cast<int>(secretFileSize),
        buffer32
    );


    /*
     * Write modified bytes
     */
    outputImage.write(buffer32, 32);


    return Status::SUCCESS;
}


/*
 * Encode secret file data
 */
Status Encoder::encodeSecretFileData()
{
    for (unsigned long int i = 0;
         i < secretFileSize;
         i++)
    {
        char buffer8[8];
        char secretByte;


        /*
         * Read 8 bytes from image
         */
        srcImage.read(buffer8, 8);


        if (!srcImage)
        {
            return Status::FAILURE;
        }


        /*
         * Read one byte from secret file
         */
        secretFile.read(&secretByte, 1);


        if (!secretFile)
        {
            return Status::FAILURE;
        }


        /*
         * Hide secret byte inside image
         */
        encodeByteToLSB(secretByte, buffer8);


        /*
         * Write modified image bytes
         */
        outputImage.write(buffer8, 8);


        if (!outputImage)
        {
            return Status::FAILURE;
        }
    }


    return Status::SUCCESS;
}


/*
 * Copy remaining image data
 */
Status Encoder::copyRemainingImageData()
{
    char ch;


    while (srcImage.read(&ch, 1))
    {
        outputImage.write(&ch, 1);
    }


    return Status::SUCCESS;
}


/*
 * Complete encoding operation
 */
Status Encoder::encode()
{
    cout << "Opening required files" << endl;

    if (openFiles() != Status::SUCCESS)
    {
        cout << "ERROR: Opening files failed" << endl;
        return Status::FAILURE;
    }

    cout << "Done Opening files" << endl;


    cout << "Checking image capacity" << endl;

    if (checkCapacity() != Status::SUCCESS)
    {
        cout << "ERROR: Image does not have enough capacity"
             << endl;

        return Status::FAILURE;
    }

    cout << "Done Checking image capacity" << endl;


    cout << "Copying BMP header" << endl;

    if (copyBmpHeader() != Status::SUCCESS)
    {
        cout << "ERROR: Copying BMP header failed"
             << endl;

        return Status::FAILURE;
    }

    cout << "Done Copying BMP header" << endl;


    cout << "Encoding Magic String" << endl;

    if (encodeMagicString(MAGIC_STRING) != Status::SUCCESS)
    {
        cout << "ERROR: Encoding magic string failed"
             << endl;

        return Status::FAILURE;
    }

    cout << "Done Encoding Magic String" << endl;


    cout << "Encoding Secret File Extension" << endl;

    if (encodeSecretFileExtension() != Status::SUCCESS)
    {
        cout << "ERROR: Encoding extension failed"
             << endl;

        return Status::FAILURE;
    }

    cout << "Done Encoding Secret File Extension"
         << endl;


    cout << "Encoding Secret File Size" << endl;

    if (encodeSecretFileSize() != Status::SUCCESS)
    {
        cout << "ERROR: Encoding secret file size failed"
             << endl;

        return Status::FAILURE;
    }

    cout << "Done Encoding Secret File Size" << endl;


    cout << "Encoding Secret File Data" << endl;

    if (encodeSecretFileData() != Status::SUCCESS)
    {
        cout << "ERROR: Encoding secret file data failed"
             << endl;

        return Status::FAILURE;
    }

    cout << "Done Encoding Secret File Data" << endl;


    cout << "Copying remaining image data" << endl;

    if (copyRemainingImageData() != Status::SUCCESS)
    {
        cout << "ERROR: Copying remaining data failed"
             << endl;

        return Status::FAILURE;
    }

    cout << "Everything is copied" << endl;

    cout << "Encoding completed successfully" << endl;


    return Status::SUCCESS;
}