#ifndef SOURCE2HTML_H
#define SOURCE2HTML_H

#include<stdio.h>

void html_begin(FILE *fp);
void html_end(FILE *fp);
void html_escape(FILE *fp,char ch);

void tokenize(FILE *src,FILE *html);

int isKeyword(char *word);

void print_keyword(FILE *fp,char *str);
void print_identifier(FILE *fp,char *str);
void print_number(FILE *fp,char *str);
void print_string(FILE *fp,char *str);
void print_comment(FILE *fp,char *str);
void print_operator(FILE *fp,char *str);
void print_preprocessor(FILE *fp,char *str);

#endif