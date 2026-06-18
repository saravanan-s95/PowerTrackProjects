#ifndef TOKEN_SPEC_H
#define TOKEN_SPEC_H
#include <stdbool.h>

#define KEYWORDS    32

// Keywords
static char *keywords[KEYWORDS] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

// Operators
static const char* operators = "+-*/%=!<>|&";

// Special Characters
static const char* special_characters = ",;{}()[]";

// Language: [_a-zA-Z]
bool letter(void);


// Language: [0-9]
bool number(void);


// Language: [' ', \n, \t]
bool whitespace(void);


// Language: (letter)(letter/number)*
bool identifier(void);


// Language: {C Keywords}
bool keyword(char *str);


// Language: {C Operators}
bool opr(void);


// Language: {C Special chars}
bool special_chars(void);


// Language: {Integer and float literal}
int integer_or_float(void);


// Language: {Char and string literal}
int char_or_string(void);

// Language: [#]
bool preprocessor(void);

#endif