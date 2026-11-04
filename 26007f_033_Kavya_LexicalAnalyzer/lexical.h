#ifndef LEXICAL_H
#define LEXICAL_H

#include <stdio.h>

#define TOKEN_SIZE 256

typedef enum
{
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    FLOAT,
    STRING,
    CHARACTER,
    OPERATOR,
    SPECIAL_SYMBOL,
    PREPROCESSOR,
    UNKNOWN,
    END_OF_FILE
} TokenType;

typedef struct
{
    TokenType type;
    char lexeme[TOKEN_SIZE];
    int line;
} Token;

typedef struct
{
    FILE *fp;
    int ch;
    int line;
} Lexer;

int lexer_init(Lexer *lexer, const char *filename);
void lexer_destroy(Lexer *lexer);
Token get_next_token(Lexer *lexer);
const char *get_token_name(TokenType type);

#endif
