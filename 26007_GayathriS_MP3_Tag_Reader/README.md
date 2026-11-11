

## Abstract
To develop a C program that reads and displays ID3 tags (Title, Artist, Album, Year, Content, composer ) from an MP3 file and allows editing of the tags.

## DESCRIPTION
MP3 files contain metadata stored as ID3 tags at the beginning or end of the file. This project parses the MP3 file in binary mode, extracts the tag information like Title, Artist, Album etc., and displays it to the user. It also provides functionality to edit and update the ID3 tags.

## INPUT
MP3 file (e.g., sample.mp3)

## OUTPUT
Displays:
- Title
- Artist  
- Album
- Year
- content
- Composer

And allows editing option

## FILES INCLUDED
- mp3_tag_reader.c - Main source code
- mp3view.c - Function to view tags
- mp3edit.c - Function to edit tags
- sample.mp3 - Sample MP3 file for testing



