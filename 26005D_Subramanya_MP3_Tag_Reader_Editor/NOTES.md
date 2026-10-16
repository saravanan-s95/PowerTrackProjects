# NOTES.md
# ID3v2.3 format notes

Reference material from building the parser/editor — kept for later reading.

## Endianness is per-field, not per-file

There's no single "endianness of a file." Each multi-byte field in a format
spec defines its own encoding:

    Bytes 0–3  -> 32-bit integer, little-endian
    Bytes 4–7  -> 32-bit integer, big-endian
    Bytes 8–11 -> special encoding (e.g. synchsafe)

Decoding flow: raw bytes -> identify which field -> look up that field's
encoding rule -> decode accordingly. Never assume.

## Why synchsafe integers exist

ID3 wants certain multi-byte values to avoid ever looking like an MPEG sync
marker inside the audio stream. A synchsafe integer restricts each byte so
the most significant bit is always 0. A decoder that understands ID3 reads
the fixed 10-byte header, gets the tag size, and skips the metadata; the
synchsafe restriction protects software that scans for MPEG sync words
without first parsing the ID3 structure (it may start scanning mid-stream).

## Header layout (10 bytes)

    ID3 03 00 00 00 10 13 19
    │   │  │  │  └──┴──┴──┴── tag size, synchsafe (7 bits/byte)
    │   │  │  └── flags
    │   └──┴── version (major.minor)
    └── file identifier "ID3"

**Important — the tag-header size is synchsafe, but individual FRAME sizes
in ID3v2.3 are NOT.** Frame sizes are plain 32-bit big-endian; synchsafe
frame sizes only start in ID3v2.4. Both fields are 4-byte, big-endian-ish,
and read with identical code, which makes mixing them up an easy bug.

## Frame layout

    4 bytes -> Frame ID (e.g. "TIT2")
    4 bytes -> Frame size (plain big-endian in v2.3)
    2 bytes -> Flags (unused here)
    N bytes -> Frame data

A frame ID byte of `0x00` means there's no more frame data — the rest of
the tag space is padding.

## Text frame data (TIT2 / TPE1 / TALB / TYER / TCON)

Byte 0 of the data is a text-encoding marker (`0x00` = ISO-8859-1,
`0x01` = UTF-16, etc). The actual text starts at byte 1.

## COMM (comment) frame data

Bigger internal header than the other text frames:

    1 byte  -> encoding
    3 bytes -> language code (e.g. "eng")
    N bytes -> short description, null-terminated
    N bytes -> actual comment text

The short description is often empty (just a null byte), which is easy to
assume is *always* true — don't; find the actual null terminator rather
than hardcoding an offset of 5, or a non-empty description silently
corrupts the parse.