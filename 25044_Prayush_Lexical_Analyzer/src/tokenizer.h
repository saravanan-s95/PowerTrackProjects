#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "types.h"

Status read_and_validate_args(char *argv[]);
Status tokenize_file(const char *filename);

#endif // TOKENIZER_H
