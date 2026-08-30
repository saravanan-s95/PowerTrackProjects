#include "types.h"
#include "lexical.h"

Status is_keyword(char word[])
{
    char *keywords[] = {"const", "volatile", "extern", "auto", "register", "static",
                        "signed", "unsigned", "short", "long", "double", "char", "int", "float", "struct",
                        "union", "enum", "void", "typedef", "goto", "return", "continue", "break", "if",
                        "else", "for", "while", "do", "switch", "case", "default", "sizeof"};

    int len = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < len; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
            return e_success;
    }

    return e_failure;
}