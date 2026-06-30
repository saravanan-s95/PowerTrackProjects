# MP3 Tag Reader

## Project Description
This project is developed in C language to read and edit ID3 tags of MP3 files.

## Features
- View MP3 Tags
- Edit Title
- Edit Artist
- Edit Album
- Edit Year
- Edit Genre
- Display Help Menu

## Compile

```bash
gcc main.c view.c edit.c -o mp3_tag
```

## Run

```bash
./mp3_tag -h
./mp3_tag -v sample.mpeg
./mp3_tag -e -t "New Title" sample.mpeg
```
