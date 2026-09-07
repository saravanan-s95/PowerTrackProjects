#include "Main.h"

/* Data type keywords */
char *dataKeywords[] =
{
    "int","float","char","double",
    "short","long","signed",
    "unsigned","void"
};

/* Non data type keywords */
char *nonDataKeywords[] =
{
    "goto","return","continue",
    "break","if","for","else",
    "while","do","switch",
    "case","default","sizeof"
};
// checks whether the given string is a data type keyword or not
int isDataKeyword(char *str)
{
    int i;

    for(i = 0; i < DATA_KEY_COUNT; i++)
    {
        if(strcmp(str, dataKeywords[i]) == 0)
            return 1;
    }

    return 0;
}
// checks whether the given string is a non data type keyword or not
int isNonDataKeyword(char *str)
{
    int i;

    for(i = 0; i < NON_DATA_KEY_COUNT; i++)
    {
        if(strcmp(str, nonDataKeywords[i]) == 0)
            return 1;
    }

    return 0;
}
//Checks whether the given character is an operator
int isOperator(char ch)
{
    return(ch == '+' || ch == '-' ||
           ch == '*' || ch == '/' ||
           ch == '%' || ch == '=' ||
           ch == '<' || ch == '>');
}
//Checks whether the given character is a special symbol used in C language
int isSpecialChar(char ch)
{
    return(ch == '(' || ch == ')' ||
           ch == '{' || ch == '}' ||
           ch == '[' || ch == ']' ||
           ch == ';' || ch == ',');
}
//Checks whether the given token is an integer literal
int isInteger(char *str)
{
    int i;

    for(i = 0; str[i] != '\0'; i++)
    {
        if(!isdigit(str[i]))
            return 0;
    }

    return 1;
}
//Checks whether the given token is a floating-point literal.
int isFloat(char *str)
{
    int i;
    int dot = 0;

    for(i = 0; str[i] != '\0'; i++)
    {
        if(str[i] == '.')
            dot++;

        else if(!isdigit(str[i]))
            return 0;
    }

    return(dot == 1);
}