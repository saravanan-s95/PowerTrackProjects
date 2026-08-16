#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>    //for functions like printf(),fgetc(),ungetc(),FILE
#include <string.h>     //strcmp()  to compare keywords
#include <ctype.h>     //isdigit(),isalpha(),isalnum(),isspace()


#define MAX_SIZE 100             //maximum token length for char word[] array

//keywords datatype
static const char *keywords_data[] =
{
    "int",
    "char",
    "float",
    "double",
    "void",
    "short",
    "long",
    "signed",
    "unsigned",
    "const",
    "volatile",
    "static",
    "extern",
    "register",
    "auto",
    "struct",
    "union",
    "enum",
    "typedef"
};

//other reserved keywords
static const char *keywords_non_data[] =
{
    "if",
    "else",
    "switch",
    "case",
    "default",
    "for",
    "while",
    "do",
    "break",
    "continue",
    "goto",
    "return",
    "sizeof"
};

//operators
static const char operators[] =
{
    '+',
    '-',
    '*',
    '/',
    '%',
    '=',
    '<',
    '>',
    '!',
    '&',
    '|',
    '^',
    '~'
};

//Delimiters
static const char delimiters[] =
{
    '(',
    ')',
    '{',
    '}',
    '[',
    ']',
    ';',
    ',',
    ':'
};

//function prototypes

int iskeyword(const char *word);

int is_nondatakeyword(const char *word);

int isoperator(char ch);

int isdelimiter(char ch);

void lexical_analysis(FILE *fin);

void print_token(const char *type, const char *value);

#endif