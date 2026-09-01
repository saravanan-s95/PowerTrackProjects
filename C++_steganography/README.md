# C++ Steganography

## 📌 Project Overview

**Steganography** is the technique of hiding secret information inside another file or media so that the existence of the hidden information is not easily noticed.

In this project, **C++ is used to implement image steganography**, where a secret message or file is encoded into an image. The hidden information can later be extracted from the encoded image.

The project demonstrates how binary data can be manipulated at the byte level using C++ file handling and bitwise operations.

---

## 🎯 Objectives

* To understand the concept of steganography.
* To hide secret information inside an image.
* To extract the hidden information from the encoded image.
* To understand image file handling in C++.
* To implement bitwise operations for data hiding.
* To practice file handling, classes, functions, and command-line arguments in C++.

---

## 🛠️ Technologies Used

* **Programming Language:** C++
* **Concepts Used:**

  * File Handling
  * Object-Oriented Programming
  * Bitwise Operations
  * Command Line Arguments
  * Binary Data Processing
  * Pointers
  * Structures/Classes
  * Dynamic Memory Allocation

---

## 📂 Project Structure

```text
C++-Steganography/
│
├── README.md
├── main.cpp
├── encode.cpp
├── encode.h
├── decode.cpp
├── decode.h
├── types.h
├── common.h
└── sample/
    ├── source.bmp
    └── output.bmp
```

> The exact file names may vary depending on your implementation.

---

## 🔐 How Steganography Works

The basic process consists of two operations:

### 1. Encoding

During encoding, the secret information is embedded into the source image.

```text
Secret Message
      ↓
Convert to Binary Data
      ↓
Read Source Image
      ↓
Modify Image Data
      ↓
Store Hidden Data
      ↓
Encoded Image
```

The image data is modified in such a way that the hidden information is stored without producing an obvious visual change in the image.

---

### 2. Decoding

During decoding, the hidden information is extracted from the encoded image.

```text
Encoded Image
      ↓
Read Image Data
      ↓
Locate Hidden Information
      ↓
Extract Binary Data
      ↓
Convert to Original Data
      ↓
Secret Message/File
```

---

## 💡 Features

* Encode secret information into an image.
* Decode hidden information from an encoded image.
* Supports binary file operations.
* Uses C++ file handling.
* Uses bit-level manipulation.
* Command-line based operation.
* Preserves the visual appearance of the image as much as practical.

---

## ▶️ Compilation

Using `g++`:

```bash
g++ main.cpp encode.cpp decode.cpp -o steganography
```

---

## 🚀 Execution

### Encoding

```bash
./steganography -e source.bmp secret.txt output.bmp
```

Where:

* `-e` → Encode operation
* `source.bmp` → Original image
* `secret.txt` → Secret file/message
* `output.bmp` → Encoded image

### Decoding

```bash
./steganography -d output.bmp extracted.txt
```

Where:

* `-d` → Decode operation
* `output.bmp` → Encoded image
* `extracted.txt` → Extracted secret file

> Use the command-line format required by your particular implementation.

---

## 🧩 Main Modules

### Encode Module

Responsible for:

* Opening the source image.
* Reading image/header information.
* Reading the secret file.
* Embedding secret data into image data.
* Creating the stego/encoded image.

### Decode Module

Responsible for:

* Opening the encoded image.
* Reading the embedded information.
* Extracting the hidden data.
* Reconstructing the original secret file.

### Main Module

Responsible for:

* Processing command-line arguments.
* Selecting encode/decode operation.
* Calling the appropriate functions.

---

## 📚 Concepts Learned

Through this project, the following concepts are practiced:

### C++ File Handling

```cpp
fstream
ifstream
ofstream
```

These are used to read and write image and secret-file data.

### Bitwise Operations

Bitwise operators are used to manipulate individual bits of data.

```cpp
&
|
^
<<
>>
```

### Command Line Arguments

The program can receive input through:

```cpp
int main(int argc, char *argv[])
```

This allows the user to specify the operation and input/output files from the terminal.

---

## 🔄 Project Workflow

### Encoding Workflow

1. Start the program.
2. Select encode operation.
3. Open the source image.
4. Validate the image format.
5. Open the secret file.
6. Read the secret data.
7. Embed the secret data into the image.
8. Generate the encoded image.
9. Save the encoded image.

### Decoding Workflow

1. Start the program.
2. Select decode operation.
3. Open the encoded image.
4. Validate the image.
5. Locate the hidden data.
6. Extract the hidden information.
7. Reconstruct the secret file.
8. Save the extracted data.

---

## 📊 Example

### Input

```text
Source Image  : source.bmp
Secret File   : secret.txt
```

Secret message:

```text
Embedded Systems
```

### Encoding

```text
source.bmp + secret.txt
          ↓
     Steganography
          ↓
      output.bmp
```

### Decoding

```text
output.bmp
    ↓
Steganography
    ↓
secret.txt
```

Extracted message:

```text
Embedded Systems
```

---

## ⚠️ Limitations

* The implementation may support only specific image formats such as BMP.
* The source image must have sufficient capacity to store the secret data.
* Modifying the encoded image can corrupt or destroy the hidden information.
* This technique is intended for information hiding and is not a replacement for encryption.

---

## 🔮 Future Enhancements

* Support additional image formats.
* Add password-based protection.
* Combine encryption with steganography.
* Provide a graphical user interface.
* Support hiding larger files.
* Improve error handling and validation.
* Add compression before hiding the secret data.

---

## 👨‍💻 Author

**Manju Thiru**

**Project:** C++ Steganography

**Domain:** C++ / File Handling / Data Hiding

---

## 📄 License

This project is created for **educational and learning purposes**.
