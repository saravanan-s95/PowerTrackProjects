#include "logs.h"
#include "tokenizer.h"
#include "types.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        LOG_ERROR(ERROR_INVALID_ARGUMENTS);
        return 1;
    }
    if (read_and_validate_args(argv) == failure) {
        LOG_ERROR(ERROR_INVALID_ARGUMENTS);
        return 1;
    }
    if (tokenize_file(argv[1]) == failure) {
        return 1;
    }
    return 0;
}
