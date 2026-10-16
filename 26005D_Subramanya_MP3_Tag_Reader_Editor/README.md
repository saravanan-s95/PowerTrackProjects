# README.md
# MP3 Tag Reader & Editor

CLI tool to view and edit ID3v2.3 metadata tags on MP3 files, written in C.

## Build

    make

Produces `./a.out`. `make clean` removes build artifacts.

## Usage

View tags:

    ./a.out -v song.mp3

Edit a tag:

    ./a.out -e <flag> "new value" song.mp3

Flags:

| Flag | Tag  | Meaning |
|------|------|---------|
| -t   | TIT2 | Title   |
| -a   | TPE1 | Artist  |
| -A   | TALB | Album   |
| -y   | TYER | Year    |
| -m   | TCON | Music/genre |
| -c   | COMM | Comment |

Help:

    ./a.out --help

## How editing works

The tag being edited is never modified in place. The tool streams the
original file into `temp.mp3`, swapping out only the target frame's data
and updating the tag's synchsafe size header, then replaces the original
file with `temp.mp3`. All other frames and the audio data are copied
through unchanged.

## Known limitations

- Only ID3v2.3 is supported. ID3v2.4 uses synchsafe frame sizes (v2.3 uses
  plain big-endian for frame sizes, only the tag header size is synchsafe)
  and won't parse correctly here.
- Text frames are assumed to use ISO-8859-1/UTF-8-compatible single-byte
  encoding (encoding byte `0x00`). UTF-16 tagged frames (`0x01`/`0x02`)
  aren't decoded specially.
- The COMM frame's short-description field is preserved when editing, but
  the tool doesn't let you set it separately — only the comment text after
  it is replaceable.

See `NOTES.md` for the ID3v2.3 format details this was built against.