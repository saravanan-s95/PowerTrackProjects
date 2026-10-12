#ifndef TOKEN_HANDLER_H
#define TOKEN_HANDLER_H

#include "types.h"
#include <stdio.h>

typedef struct BracketTracker {
    int open_curly;
    int close_curly;
    int open_paren;
    int close_paren;
    int open_square;
    int close_square;
} BracketTracker;

void print_token(TokenType type, const char *token);

Status handle_alnum_token(FILE *fptr, int first_char, int *line, int *col);
Status handle_string_token(FILE *fptr, int quote_char, int *line, int *col);
Status handle_operator_symbol_token(FILE *fptr, int first_char, int *line, int *col);

void handle_bracket(int ch, BracketTracker *tracker);
Status validate_brackets(const BracketTracker *tracker);

#endif // TOKEN_HANDLER_H
