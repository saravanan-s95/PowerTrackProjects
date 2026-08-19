#include <stdio.h>
#include <string.h>
#include "keyword.h"

// List of C keywords
char *keywords[] =
{
    "int",
    "char",
    "float",
    "double",
    "void",
    "if",
    "else",
    "for",
    "while",
    "do",
    "switch",
    "case",
    "default",
    "break",
    "continue",
    "return",
    "struct",
    "typedef",
    "const",
    "static"
};

// Check keyword
int is_keyword(char *word)
{
    int i;

    for(i = 0; i < 20; i++)
    {
        if(strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}