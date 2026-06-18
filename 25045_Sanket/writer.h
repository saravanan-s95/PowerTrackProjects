#ifndef WRITER_H
#define WRITER_H

#include "utils.h"

/* Write complete tag structure back to MP3 */
int write_id3_tags(const char *filename,
                   const TagData *data);

/* Edit a single field */
int edit_tag(const char *filename,
             const char *tag,
             const char *value);

/* Delete one tag */
int delete_tag(const char *filename,
               const char *tag);

/* Remove all ID3 metadata */
int delete_all_tags(const char *filename);

/* Extract embedded album art */
int extract_album_art(const char *mp3_filename,
                      const char *output_image_name);

#endif