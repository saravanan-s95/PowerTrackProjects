#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Maximum size of a token */
#define MAX_TOKEN_SIZE 100

/* Token types */
typedef enum
{
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_CONSTANT,
    TOKEN_OPERATOR,
    TOKEN_SPECIAL_CHARACTER,
    TOKEN_STRING,
    TOKEN_CHARACTER,
    TOKEN_HEADER,
    TOKEN_UNKNOWN
} TokenType;

/* Token structure */
typedef struct
{
    TokenType type;
    char value[MAX_TOKEN_SIZE];
} Token;

/* Function declarations */

void lexical_analyzer(FILE *fp);

int is_keyword(const char *word);

int is_special_character(char ch);

int is_identifier_start(char ch);

int is_identifier_char(char ch);

int is_operator(char ch);

void print_token(TokenType type, const char *value);

#endif