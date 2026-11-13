# Image Steganography Using C

## Project Description
This project implements image steganography in C using the Least Significant Bit (LSB) technique to hide secret file data inside a BMP image and extract it during decoding.

## Features
- Encode secret data into a BMP image.
- Decode hidden data from the stego image.
- Validate input files and command-line arguments.
- Store and extract the secret file extension, size, and contents.

## Technologies Used
- C Programming
- GCC Compiler
- File Handling
- Bitwise Operations
- BMP Image Format

## Compilation
```bash
gcc *.c -o steganography
```

## Usage

### Encoding
```bash
./steganography -e beautiful.bmp secret.txt stego.bmp
```

### Decoding
```bash
./steganography -d stego.bmp decoded.txt
```

## Verification
The original and decoded files were compared using `cmp`, and the files matched successfully.

## Author
Sheeba Insily R Robert

## Batch
26013
