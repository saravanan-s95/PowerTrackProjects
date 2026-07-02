#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define MAX_TOKEN_LEN 100
#define MAX_LINE_LEN 256
#define MAX_KEYWORDS 32
#define MAX_OPERATORS 30

// Token types
typedef enum {
    KEYWORD,
    IDENTIFIER,
    NUMERIC_CONSTANT,
    STRING_CONSTANT,
    CHARACTER_CONSTANT,
    OPERATOR,
    PREPROCESSOR_DIRECTIVE,
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    SEMICOLON,
    COMMA,
    DELIMITER,
    UNKNOWN
} TokenType;

// Token structure
typedef struct {
    char value[MAX_TOKEN_LEN];
    TokenType type;
    int line_number;
} Token;

// Function declarations
void init_lexer();
void analyze_file(const char* filename);
Token get_next_token(FILE* file, int* line_num);
int is_keyword(const char* str);
int is_operator(const char* str);
int is_numeric(const char* str);
int is_valid_identifier(const char* str);
void print_token(Token token);
const char* token_type_to_string(TokenType type);

// Global arrays
extern const char* keywords[];
extern const char* operators[];
extern const int num_keywords;
extern const int num_operators;

#endif