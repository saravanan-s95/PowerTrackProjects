#include "logs.h"
#include "token_handler.h"
#include "tokenizer.h"
#include "types.h"
#include "validations.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

Status read_and_validate_args(char *argv[]) {
    LOG_INFO(INFO_VALIDATE_ARGUMENTS);
    char *ext = strrchr(argv[1], '.');
    if (ext == NULL || strcmp(ext, ".c") != 0)
        return failure;
    return success;
}

Status tokenize_file(const char *filename) {
    LOG_INFO(INFO_OPEN_FILE);
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        LOG_ERROR(ERROR_OPEN_FILE);
        return failure;
    }

    int ch;
    int line = 1, col = 1;
    BracketTracker bracket_tracker = {0};

    while ((ch = fgetc(fptr)) != EOF) {
        if (ch == '\n') {
            line++;
            col = 1;
            continue;
        }

        // Skip preprocessor directives (#...)
        if (ch == '#') {
            col++;
            while ((ch = fgetc(fptr)) != EOF && ch != '\n') {
                col++;
            }
            if (ch == '\n') {
                line++;
                col = 1;
            }
            continue;
        }

        // Skip comments (// and /* ... */)
        if (ch == '/') {
            int next_ch = fgetc(fptr);
            if (next_ch == '/') {
                col += 2;
                while ((ch = fgetc(fptr)) != EOF && ch != '\n') {
                    col++;
                }
                if (ch == '\n') {
                    line++;
                    col = 1;
                }
                continue;
            } else if (next_ch == '*') {
                col += 2;
                int prev = 0;
                while ((ch = fgetc(fptr)) != EOF) {
                    if (ch == '\n') {
                        line++;
                        col = 1;
                    } else {
                        col++;
                    }
                    if (prev == '*' && ch == '/')
                        break;
                    prev = ch;
                }
                continue;
            } else {
                if (next_ch != EOF)
                    ungetc(next_ch, fptr);
            }
        }

        // Track brackets
        if (ch == '{' || ch == '}' || ch == '(' || ch == ')' || ch == '[' || ch == ']') {
            handle_bracket(ch, &bracket_tracker);
        }

        // Token processing using character branch statements
        if (ch == '"' || ch == '\'') {
            if (handle_string_token(fptr, ch, &line, &col) == failure) {
                fclose(fptr);
                return failure;
            }
        } else if (isalnum(ch) || ch == '_') {
            if (handle_alnum_token(fptr, ch, &line, &col) == failure) {
                fclose(fptr);
                return failure;
            }
        } else if (ch == '.') {
            int next_ch = fgetc(fptr);
            if (next_ch != EOF)
                ungetc(next_ch, fptr);
            if (next_ch != EOF && isdigit(next_ch)) {
                if (handle_alnum_token(fptr, ch, &line, &col) == failure) {
                    fclose(fptr);
                    return failure;
                }
            } else {
                if (handle_operator_symbol_token(fptr, ch, &line, &col) == failure) {
                    fclose(fptr);
                    return failure;
                }
            }
        } else if (!isspace(ch)) {
            if (handle_operator_symbol_token(fptr, ch, &line, &col) == failure) {
                fclose(fptr);
                return failure;
            }
        } else {
            col++;
        }
    }

    fclose(fptr);

    if (validate_brackets(&bracket_tracker) == failure) {
        return failure;
    }

    return success;
}
