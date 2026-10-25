# LSB Image Steganography (C++)

A small command-line tool that hides a secret file inside a BMP image and gets it back out again. It works by tucking the secret's bits into the least significant bit of the image's pixel bytes, so the picture looks the same to the eye but quietly carries your data.

## How it works

- Every byte of the secret is spread across the LSBs of 8 consecutive image bytes.
- The first 54 bytes (the BMP header) are copied untouched.
- Data is written in this order:
  1. Magic string (`26001A`), so the decoder knows the image is actually encoded
  2. Secret file extension size (4 bytes)
  3. Secret file extension (e.g. `.txt`)
  4. Secret file size (4 bytes)
  5. Secret file data
- The rest of the image is copied as-is.
- Before encoding, the tool checks that the image has enough capacity for everything above.

## Project structure

| File | Purpose |
|------|---------|
| `main.cpp` | Entry point, picks encode or decode from the arguments |
| `encode.cpp / encode.h` | `Encoder` class: validation, capacity check, encoding |
| `decode.cpp / decode.h` | `Decoder` class: validation, magic string check, decoding |
| `common.h` | Magic string and operation-type check |
| `types.h` | Shared types (`Status`, `OperationType`, `uint`) |

## Build

```bash
g++ *.cpp
```

## Usage

### Encode

```bash
./a.out -e <source.bmp> <secret_file> [output.bmp]
```

```bash
./a.out -e beautiful.bmp secret.txt stego.bmp
```

If you skip the output name, it defaults to `encode_output_fname.bmp`.

### Decode

```bash
./a.out -d <stego.bmp> [output_name]
```

```bash
./a.out -d stego.bmp decode
```

If you skip the output name, the result is saved as `decode` plus the original extension (e.g. `decode.txt`). The extension is recovered from the image, so you don't need to type it.

## Things to keep in mind

- The source image must be a **24-bit uncompressed `.bmp`**. Other formats like `.jpg` won't work.
- The secret file needs an extension (e.g. `secret.txt`).
- A bigger secret needs a bigger image. If it doesn't fit, encoding stops with a capacity error.
- If the image wasn't encoded by this tool, decoding fails at the magic string check.

## Example

```
$ ./a.out -e beautiful.bmp secret.txt
$ ./a.out -d encode_output_fname.bmp
$ cat decode.txt
My password is SECRET
```
