#ifndef LEXER_H
#define LEXER_H

#include "types.h"
#include <stdio.h>

typedef struct
{
    char token[100];
    char type[30];
    int line_no;
    int count;
} TokenInfo;

typedef struct
{
    int line_no;
    char error_msg[200];
} ErrorInfo;

typedef struct
{
    FILE *fptr_file_c;
    char *C_file_name;

    int line_no;

    TokenInfo tokens[1000];
    int token_count;

    ErrorInfo errors[100];
    int error_count;

} Lexer;

/* Function prototype */
Status read_and_validate_args(char *argv[], Lexer *lexer);
Status analyze_file(Lexer *lexer);

/* File handling */
Status open_file(Lexer *lexer);

Status extract_tokens(Lexer *lexer);

Status skip_preprocessor(Lexer *lexer);
Status skip_comments(Lexer *lexer, char first);

Status read_identifier_or_keyword(Lexer *lexer, char first);
Status read_number(Lexer *lexer, char first);
Status read_character_literal(Lexer *lexer, char first);
Status read_string_literal(Lexer *lexer, char first);
Status read_operator(Lexer *lexer, char first);

int is_keyword(char *token);
int is_symbol(char ch);
int is_operator(char ch);

Status classify_symbol(Lexer *lexer, char ch);
Status classify_tokens(char *token, Lexer *lexer);

/*Store and display tokens*/
Status store_token(Lexer *lexer, char *token, char *type);
void display_tokens(Lexer *lexer);

#endif