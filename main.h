#ifndef LEXER_H
#define LEXER_H

//HEADER FILES
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define SUCCESS 1
#define FAILURE 0
//define a macro name for colors
#define RED       "\033[31m"  
#define RESET     "\033[0m"
#define GREEN     "\033[32m"
#define BBLUE     "\033[94m"
#define BCYAN     "\033[96m"
#define BMAGENTA  "\033[95m"
#define ORANGE    "\033[38;5;208m"

//FUNCTION PROTOTYPES
void lexical_analyzer(FILE *fptr);
int is_keyword(char *word);
int is_identifier(char *word);
int is_number(char *word);
int is_operator(char ch, FILE *fptr,int line_number);
void is_preprocessor(char ch,FILE *fptr,int line_number);
int is_delimiter(char ch);
int test();
int is_float(char *word);
int is_invalid_identifier(char *word);
int is_hexadecimal(char *word,int *hex_flag,int line_number);
int is_octal(char *word, int line_number);
void check_brackets(char ch, int line_number);
void check_remaining_brackets(int line_number);

extern char errors[100][100];   //array to store errors.
extern int error_line[100];     //array to store line numbers where errors are present.
extern int error_count;     //error count.

#endif