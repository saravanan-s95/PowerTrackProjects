#ifndef MP3_H
#define MP3_H
#include "helpers.h"
#include "types.h"
#include <stdio.h>

typedef struct Header {
    char magic[4];
    unsigned char version;
    char revision;
    char flags;
    unsigned int size;
} Header;

typedef struct Frame {
    char tag[5];
    unsigned int size;
    char content[100];
} Frame;

typedef struct Metadata {
    Header header;
    Frame title;
    Frame artist;
    Frame year;
    Frame album;
    Frame composer;
    Frame content;
    int tagCount;
} Metadata;

Status readHeaders(Header *header, FILE *file);
Status readFrame(Frame *frame, FILE *file, unsigned char version, long tagEnd);
Status readMetadata(Metadata *metadata, FILE *file);
Status editFrame(const char *tag, MP3_info *info);
#endif // !MP3_H
