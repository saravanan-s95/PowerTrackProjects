#include "functions.h"
#include <string.h>



int is_keyword(char token[])
{
    char *keywords[] = {
        "auto", "break", "case", "char", "const",
        "continue", "default", "do", "double", "else",
        "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return",
        "short", "signed", "sizeof", "static", "struct",
        "switch", "typedef", "union", "unsigned",
        "void", "volatile", "while"
    };

    int i;

    for (i = 0; i < 32; i++)
    {
        if (strcmp(token, keywords[i]) == 0)
        {
            return SUCCESS;
        }
    }

    return FAILURE;
}

int is_digit(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return SUCCESS;
    }

    return FAILURE;
}



int is_operator(char ch)
{
    char operators[] = "+-*/%=<>!&|^~(){}[];,:.";

    int i;

    for (i = 0; operators[i] != '\0'; i++)
    {
        if (ch == operators[i])
        {
            return SUCCESS;
        }
    }

    return FAILURE;
}

int is_assignment(char ch)
{
    if (ch == '=')
    {
        return SUCCESS;
    }

    return FAILURE;
}

int is_octal(char token[])
{
    int i;

    if (token[0] != '0' || token[1] == '\0')
    {
        return FAILURE;
    }

    for (i = 1; token[i] != '\0'; i++)
    {
        if (token[i] < '0' || token[i] > '7')
        {
            return FAILURE;
        }
    }

    return SUCCESS;
}

int is_hexa(char token[])
{
    int i;

    if (token[0] != '0' ||
        (token[1] != 'x' && token[1] != 'X') ||
        token[2] == '\0')
    {
        return FAILURE;
    }

    for (i = 2; token[i] != '\0'; i++)
    {
        if (!((token[i] >= '0' && token[i] <= '9') ||
              (token[i] >= 'a' && token[i] <= 'f') ||
              (token[i] >= 'A' && token[i] <= 'F')))
        {
            return FAILURE;
        }
    }

    return SUCCESS;
}
