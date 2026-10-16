#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LEXEME 1024
#define VALIDATION_ERROR "Invalid CLA Input\nUSAGE: ./build/lexer input_file.c [output_file.html]\n\n"

extern int show_lines;

typedef enum
{
    SUCCESS,
    FAILURE
} status_e;

typedef enum token_type
{
    TOK_TYPE_KEYWORD,
    TOK_CONTROL_KEYWORD,
    TOK_DIRECTIVE,
    TOK_IDENTIFIER,
    TOK_ARRAY,
    TOK_INT_CONSTANT,
    TOK_FLOAT_CONSTANT,
    TOK_CHAR_LITERAL,
    TOK_STRING_LITERAL,
    TOK_OPERATOR,
    TOK_SYMBOL,
    TOK_INVALID,
    TOK_COMMENT,
    TOK_HEADER,
    TOK_WHITESPACE,
    TOK_EOF
} token_type_e;

typedef struct token
{
    FILE *fp;
    char lexeme[MAX_LEXEME];
    token_type_e type;
} token_t;

/* Lookup tables (defined in tables.c) */
extern char *type_keywords[];
extern char *control_keywords[];
extern char *directives[];
extern char *compound_operators[];
extern char operators[];
extern char symbol[];

/* Function declarations */
status_e get_token(token_t *tok);

int is_keyword(int ch, token_t *tok);
void is_array(int ch, token_t *tok);
status_e is_numeric_constant(int ch, token_t *tok);
status_e is_literal(int ch, token_t *tok);
status_e is_comment(int ch, token_t *tok);
status_e is_operator_or_symbol(int ch, token_t *tok);
status_e is_compound_helper(char *str);
status_e is_directive(int ch, token_t *tok);
status_e is_header(int ch, token_t *tok);

const char *type_name(token_type_e t);
void print_banner(FILE *out, const char *filename);
void print_header(FILE *out);
void print_token(FILE *out, const token_t *tok);
void print_summary(FILE *out, const int count[], int total);
void print_html_begin(FILE *out);
void print_html_end(FILE *out);
void print_html_token(FILE *out, const token_t *tok);
void print_html_char(FILE *out, const char ch);

#endif
