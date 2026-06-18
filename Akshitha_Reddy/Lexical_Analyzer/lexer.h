#ifndef LEXER_H
#define LEXER_H

int isDataKeyword(char *str);
int isnonDataKeyword(char *str);
int isOperator(char ch);
int isSpclChar(char ch);
int isInteger(char *str);
int isFloat(char *str);

#endif