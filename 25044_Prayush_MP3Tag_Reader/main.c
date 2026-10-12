#include "edit.h"
#include "helpers.h"
#include "logs.h"
#include "render.h"
#include "types.h"
#include "view.h"
#include <string.h>

OperationType check_operation_type(char *arg) {
    if (strcmp(arg, "-e") == 0)
        return edit_mp3;
    else if (strcmp(arg, "-v") == 0)
        return view_mp3;
    else
        return unsupported_mp3;
}

// add validations
int main(int argc, char *argv[]) {

    if (argc < 3) {
        LOG_ERROR(ERR_INVALID_ARGUMENTS);
        print_usage("MP3_TagReader");
        return 0;
    }

    OperationType ret = check_operation_type(argv[1]);

    if (ret == unsupported_mp3) {
        LOG_ERROR(ERR_INVALID_ARGUMENTS);
        print_usage("MP3_TagReader");
        return 0;
    } else if (ret == view_mp3) {
        if (argc != 3) {
            LOG_ERROR(ERR_INVALID_ARGUMENTS);
            print_usage("MP3_TagReader");
            return 0;
        }
        MP3_info info;
        if (read_and_validate_viewMP3_args(argv, &info) == failure) {
            LOG_ERROR(ERR_INVALID_ARGUMENTS);
            print_usage("MP3_TagReader");
            return 0;
        }
        if (viewTags(&info) == failure) {
            LOG_ERROR(ERROR_READ_TAG_FAILED);
            return 0;
        }
    } else {
        if (argc != 5) {
            LOG_ERROR(ERR_INVALID_ARGUMENTS);
            print_usage("MP3_TagReader");
            return 0;
        }
        MP3_info info;
        if (read_and_validate_editMP3_args(argv, &info) == failure) {
            LOG_ERROR(ERR_INVALID_ARGUMENTS);
            print_usage("MP3_TagReader");
            return 0;
        }
        if (editTags(&info) == failure) {
            LOG_ERROR(ERROR_READ_TAG_FAILED);
            return 0;
        }
        printMessage("Tag Edited Successfully");
    }
}
