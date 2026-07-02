#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>  // Added for exit()
#include "lexer.h"

// Error types
typedef enum {
    ERR_INVALID_IDENTIFIER,      // e.g., 1abc
    ERR_INVALID_HEX_NUMBER,      // e.g., 0xABCG
    ERR_INVALID_OCTAL_NUMBER,    // e.g., 0009
    ERR_INVALID_FLOAT_NUMBER,    // e.g., 12.74a
    ERR_INVALID_BINARY_NUMBER,   // e.g., 0b41
    ERR_UNCLOSED_STRING,         // "not closed
    ERR_UNCLOSED_CHAR,           // 'not closed
    ERR_UNCLOSED_PAREN,          // ( not closed
    ERR_UNCLOSED_BRACE,          // { not closed
    ERR_UNCLOSED_BRACKET,        // [ not closed
    ERR_UNKNOWN_TOKEN,           // Unknown token
    ERR_MAX_ERRORS               // For array sizing
} ErrorType;

// Error structure
typedef struct {
    ErrorType type;
    char message[256];
    int line_number;
    char token_value[MAX_TOKEN_LEN];
} Error;

// Function declarations
void report_error_and_exit(ErrorType type, int line_num, const char* token_value);
int is_valid_binary(const char* str);
int is_valid_hex(const char* str);
int is_valid_octal(const char* str);
int is_valid_float(const char* str);
int validate_token_and_exit(Token token);

#endif