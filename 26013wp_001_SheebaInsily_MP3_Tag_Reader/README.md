# MP3 Tag Reader and Editor

## Project Description
The MP3 Tag Reader and Editor is a C programming project that reads and edits ID3 metadata tags in MP3 audio files. It allows users to view existing metadata and modify supported tag information.

## Features
- View MP3 metadata.
- Display song title, artist, album, year, genre, and track number.
- Display comment and copyright information when available.
- Check for album art information.
- Edit supported ID3 tags.
- Validate command-line arguments.

## Technologies Used
- Programming Language: C
- Concepts: File Handling, Structures, Pointers, Command-Line Arguments
- Build Tool: GCC and Makefile
- Metadata Format: ID3

## Project Files
- `main.c` – Main program and command-line argument handling.
- `mp3_tag_reader.c` – MP3 tag reading and editing implementation.
- `mp3_tag_reader.h` – Function declarations.
- `types.h` – Type definitions.
- `Makefile` – Compiles the project.

## Compilation
Run the following command in the project directory:

```bash
make
```

## Usage

### View MP3 Tags
```bash
./mp3_tag_reader -v "song.mp3"
```

Replace `song.mp3` with the path to your MP3 file.

### Clean the Executable
```bash
make clean
```

## Learning Outcomes
- Understanding file handling in C.
- Working with structures and pointers.
- Reading binary file data.
- Understanding ID3 metadata frames.
- Using command-line arguments and Makefile for compilation.

## Author
**Sheeba Insily R Robert**

Batch Number: 26013
