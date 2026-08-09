# MP3 Tag Reader

A C-based command-line application to view and edit metadata (ID3 tags) in MP3 files.

## Features

* View MP3 metadata (Title, Artist, Album, Year, Track, Genre, Comment)
* Edit title, artist, album, year, track, genre and comment
* Displays the ID3 tag version detected in the file
* Validates command-line arguments and file format before processing
* Preserves the MP3 audio data while editing metadata

## Build

```bash
make
```

This produces `mp3_tag_reader.out`.

To remove build artifacts:

```bash
make clean
```

## Usage

### View MP3 metadata

```bash
./mp3_tag_reader.out -v sample.mp3
```

### Edit MP3 metadata

```bash
./mp3_tag_reader.out -e <option> <new_value> <mp3_file>
```

### Edit options

| Option | Metadata |
| ------ | -------- |
| `-t`   | Title    |
| `-T`   | Track    |
| `-a`   | Artist   |
| `-A`   | Album    |
| `-y`   | Year     |
| `-c`   | Comment  |
| `-g`   | Genre    |

### Example

```bash
./mp3_tag_reader.out -e -t "New Song Title" sample.mp3
```

### Help

```bash
./mp3_tag_reader.out --help
```

## Supported Format

Editing is only supported for files carrying an ID3v2.3 tag. Files with only
an ID3v1/ID3v1.1 tag can be viewed but not edited.

## Error Handling

* Prints an error if the file cannot be opened.
* Prints an error if the file does not have a `.mp3` extension.
* Prints an error if no ID3 tag is found in the file.
* Prints an error and lists valid options if an invalid edit option is given.