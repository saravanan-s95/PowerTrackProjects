#define LEXER_H

#include<stdio.h>
#include<string.h>
#include<ctype.h>

int is_keyword(char *word);

int is_operator(char ch);

int is_specialsymbol(char ch);

int is_number(char word[]);

int isidentifier(char word[]);

void lexicalAnalyzer(FILE *fp);
