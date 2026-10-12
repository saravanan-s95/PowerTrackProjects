#ifndef VIEW_H
#define VIEW_H
#include "helpers.h"
Status read_and_validate_viewMP3_args(char *argv[], MP3_info *info);
Status viewTags(MP3_info *info);
#endif // !VIEW_H
