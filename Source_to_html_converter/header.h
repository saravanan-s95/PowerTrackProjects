#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

static char *keywords_data[] = {"const", "volatile", "extern", "auto", "register", "static", "signed", "unsigned", "short", "long",
                                "double", "char", "int", "float", "struct", "union", "enum", "void", "typedef", NULL};

static char *keywords_non_data[] = {"goto", "return", "continue", "break", "if", "else", "for", "while", "do", "switch", "case", "default", "sizeof", NULL};

int isKeyword_data(char *word);
int isKeyword_non_data(char *word);

void handle_identifiers(FILE *in, FILE *out, char ch);
void handle_numbers(FILE *in, FILE *out, char ch);
void handle_string(FILE *in, FILE *out, char ch);
void handle_comment(FILE *in, FILE *out, char ch);
void handle_char(FILE *in, FILE *out, char ch);
void handle_preprocessor(FILE *in, FILE *out, char ch);

#endif
