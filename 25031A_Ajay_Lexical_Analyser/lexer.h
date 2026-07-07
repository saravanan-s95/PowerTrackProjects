#ifndef lexer_h
#define lexer_h

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdio_ext.h>

int keyword(char *str);
int read_fun(char *filename);
int ident(char *buffer);
int isitfloat(char *buffer);
int isithexa(char *buffer);
int isitoct(char *buffer);
int isitbin(char *buffer);
int isitdec(char *buffer);
int isitcl(char *buffer);
#endif