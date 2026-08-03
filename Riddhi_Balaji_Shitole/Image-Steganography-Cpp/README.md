# Image Steganography Using C++

A command-line Image Steganography application developed in **C++** that hides and extracts secret text inside **24-bit BMP images** using the **Least Significant Bit (LSB)** technique.

The project demonstrates file handling, object-oriented programming, bitwise operations, and image processing concepts.

---

##  Project Overview

Steganography is the process of hiding secret information inside another file so that the existence of the hidden information is concealed.

This project embeds the contents of a text file into a BMP image by modifying the Least Significant Bit (LSB) of each image byte. The hidden data can later be extracted using the decoding functionality.

---

##  Features

- Encode secret text into a BMP image
- Decode hidden text from a stego BMP image
- Command-line interface
- Image capacity verification
- Magic string verification
- Secret file extension encoding
- Secret file size encoding
- LSB (Least Significant Bit) based encoding
- Object-Oriented Design using C++
- GNU Make support

---

## Technologies Used

- C++
- Object-Oriented Programming (OOP)
- File Handling
- Bitwise Operations
- BMP Image Processing
- GNU Make
- Linux / Ubuntu / WSL

---

## Project Structure

Image-Steganography-Cpp/
│
├── Makefile
├── README.md
│
├── common.h
├── utility.h
├── utility.cpp
│
├── image.h
├── image.cpp
│
├── encoder.h
├── encoder.cpp
│
├── decoder.h
├── decoder.cpp
│
├── main.cpp
│
├── beautiful.bmp
├── secret.txt
├── stego.bmp
├── output.txt
│
└── stego


##  How the Project Works

### Encoding

The encoder stores information in the following order:

```
BMP Header (54 Bytes)
        ↓
Magic String
        ↓
Extension Length
        ↓
File Extension
        ↓
Secret File Size
        ↓
Secret File Data
        ↓
Remaining Image Data

### Decoding

The decoder reads the information in the same order:

BMP Header
        ↓
Magic String Verification
        ↓
Read Extension
        ↓
Read Secret File Size
        ↓
Extract Secret Data
        ↓
Generate Output File

## Build Instructions

### Using g++

Compile manually:

```bash
g++ *.cpp -o stego
```

## Running the Project

### Encoding

Syntax

bash
./stego -e <input.bmp> <secret.txt> <output.bmp>

Example

bash
./stego -e beautiful.bmp secret.txt stego.bmp

Sample Output
=====================================
     Image Steganography in C++
=====================================

===== Encoding Started =====
Image Capacity  : 2359297 bytes
Secret File Size: 43 bytes
Image has sufficient capacity.
Encoding Magic String...
Encoding File Extension...
Encoding Secret File Size...
Encoding Secret File...
Encoding Completed.

Encoding Successful.

### Decoding

Syntax

```bash
./stego -d <stego.bmp> <output.txt>

Example

bash
./stego -d stego.bmp output.txt

Sample Output

=====================================
     Image Steganography in C++
=====================================

===== Decoding Started =====
Decoding Magic String...
Magic String Verified.
Decoding File Extension...
Extension : .txt
Decoding Secret File Size...
Secret File Size : 43 bytes
Decoding Secret File...
Secret File Extracted Successfully.
Decoding Completed.

Decoding Successful.


## Example

Input Secret File

Hello, this is my secret message.

After encoding

beautiful.bmp
        ↓
Encoding
        ↓
stego.bmp

After decoding

stego.bmp
        ↓
Decoding
        ↓
output.txt

Contents of **output.txt**

Hello, this is my secret message.

## Concepts Used

- Object-Oriented Programming
- Classes and Objects
- File Handling
- Binary File Processing
- Bitwise Operations
- LSB Algorithm
- BMP Image Format
- Command Line Arguments
- Modular Programming





