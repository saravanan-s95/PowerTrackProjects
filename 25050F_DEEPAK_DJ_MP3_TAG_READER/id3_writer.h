#ifndef ID3_WRITER_H
#define ID3_WRITER_H

#include "id3_utils.h"

/**
 * @brief Writes the ID3 tags to an MP3 file.
 * 
 * @param filename The name of the MP3 file.
 * @param data Pointer to the TagData structure containing the ID3 tags.
 * @return 0 on success, non-zero on failure.
 */
//int write_id3_tags(const char *filename, const TagData *data);
int check_id3_tags_writer(FILE *fpread);
int get_frame_size(FILE *);
int get_size(FILE *fp);
int write_tag_unicode(char *value,int tag_size,FILE *fpwrite,int n);
int write_tag(char *value,int tag_size,FILE *fpwrite,int n);
char *check_tag(char *tag);
char *check_tag_name(char *tag);
/**
TODO: Add documention as sample given above
 */
int edit_tag(const char *filename,  char *tag, char *value);

#endif // ID3_WRITER_H
