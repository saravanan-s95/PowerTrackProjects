#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define DATA_KEY_COUNT 9       // Number of data type keywords
#define NON_DATA_KEY_COUNT 13  // Number of non-data type keywords


/* Checks whether the given token is a data type keyword */
int isDataKeyword(char *str);

/* Checks whether the given token is a non-data type keyword */
int isNonDataKeyword(char *str);

/* Checks whether the given character is an operator */
int isOperator(char ch);

/* Checks whether the given character is a special character */
int isSpecialChar(char ch);

/* Checks whether the given token is an integer literal */
int isInteger(char *str);

/* Checks whether the given token is a floating-point literal */
int isFloat(char *str);

#endif