#ifndef EDIT_H
#define EDIT_H

#include "helpers.h"
Status read_and_validate_editMP3_args(char *argv[], MP3_info *info);
Status editTags(MP3_info *info);
#endif // !EDIT_H
