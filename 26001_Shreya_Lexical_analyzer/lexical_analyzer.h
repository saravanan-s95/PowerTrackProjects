#ifndef LEXER_H
#define LEXER_H
#define SUCCESS 0
#define FAILURE 1
#define MAX_TOKEN_SIZE 100


int lexer(char*);
int isKeyword(char* str);
int isOperator(char* str);
int isBracket(char token);
#endif