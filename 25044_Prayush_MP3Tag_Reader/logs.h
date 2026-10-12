#ifndef LOGS_H
#define LOGS_H
#include <stdio.h>
#define ERR_INVALID_ARGUMENTS "Invalid Arguments."
#define ERR_INVALID_FILE "Invalid MP3 file."
#define ERR_INVALID_TAG "Invalid tag chosen for edit."
#define ERROR_READ_TAG_FAILED "Failed to Read Tags"
#define ERROR_EDIT_TAG_FAILED "Failed to Edit Tags"
#define LOG_ERROR(ERROR) printf("ERROR: %s\n", ERROR);
#endif // !LOGS_H
