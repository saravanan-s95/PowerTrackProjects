# Steganography Using LSB Technique (C++)

## Project Overview

This project implements Image Steganography using the Least Significant Bit (LSB) technique in C++. The application hides a secret text file inside a BMP image and later extracts the hidden information from the stego image.

## Files

* main.cpp
* Encode.cpp
* Encode.hpp
* Decode.cpp
* Decode.hpp
* common.hpp
* types.hpp

## Compilation

```bash
g++ main.cpp Encode.cpp Decode.cpp -o stego
```

## Encoding

```bash
./stego -e beautiful.bmp secret.txt stego.bmp
```

## Decoding

```bash
./stego -d stego.bmp output.txt
```

## Encoding Process

1. Open source BMP image and secret file
2. Check image capacity
3. Copy BMP header
4. Encode magic string
5. Encode secret file extension size
6. Encode secret file extension
7. Encode secret file size
8. Encode secret file data
9. Copy remaining image data

## Decoding Process

1. Open stego image
2. Skip BMP header
3. Verify magic string
4. Decode extension size
5. Decode extension
6. Decode secret file size
7. Decode secret file data
8. Store decoded data in output file

## Sample Input

```
My password is SECRET ;)
```

## Sample Output

```
My password is SECRET ;)
```

## Concepts Used

* Object Oriented Programming (OOP)
* Classes and Objects
* File Handling
* BMP Image Processing
* Least Significant Bit (LSB) Encoding
* Data Hiding and Extraction



