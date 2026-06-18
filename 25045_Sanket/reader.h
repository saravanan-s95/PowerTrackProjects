#ifndef ID3_READER_H
#define ID3_READER_H

#include "utils.h"

/* Read complete ID3 metadata */
TagData *read_id3_tags(const char *filename);

/* Display metadata */
void display_metadata(const TagData *data);

/* Convenience wrapper */
void view_tags(const char *filename);

/* Text decoding helpers */
void utf16_print(const unsigned char *buffer, uint32_t size);
char *decode_text_frame(const unsigned char *buffer, uint32_t size);

/* Internal validation */
int validate_frame_header(const unsigned char *frame_header);

#endif