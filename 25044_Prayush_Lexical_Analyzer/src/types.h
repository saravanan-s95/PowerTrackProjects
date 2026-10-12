#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

typedef enum Status { failure, success } Status;

typedef enum TokenType {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_SYMBOL,
    TOKEN_STRING_LITERAL,
    TOKEN_CONSTANT_LITERAL,
    TOKEN_UNKNOWN
} TokenType;

typedef struct Token {
    TokenType type;
    char token[100];
    int line;
    int col;
} Token;

#endif // TYPES_H
