#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define SUCCESS 1
#define FAILURE 0


int is_keyword(char token[]);
int is_digit(char ch);
int is_operator(char ch);
int is_assignment(char ch);
int is_octal(char token[]);
int is_hexa(char token[]);


#endif