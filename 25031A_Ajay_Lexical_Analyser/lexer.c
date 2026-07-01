#include "lexer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *keywords[32] = {"int", "float", "char", "double", "signed", "unsigned", "long",
                      "short", "volatile", "const", "for", "while", "do", "break", "goto", "continue",
                      "if", "else", "switch", "case", "default", "auto", "static", "register",
                      "extern", "struct", "union", "enum", "typedef", "void", "return", "sizeof"};

int keyword(char *buffer)
{
    for (int i = 0; i < 32; i++)
    {
        if (!strcmp(keywords[i], buffer))
        {
            return 1;
        }
    }
    return 0;
}

int ident(char *buffer)
{
    int i = 1;
    if (isdigit(buffer[0]))
    {
        while (isalnum(buffer[i]))
        {
            i++;
        }
        return 0;
    }
    else
    {
        return 1;
    }
}
int isitfloat(char *buffer)
{
    for (int i = 0; buffer[i]; i++)
    {
        if (!(isdigit(buffer[i]) || buffer[i] == '.' || buffer[i] == 'F' || buffer[i] == 'f'))
            return 0;
    }
    return 1;
}
int isithexa(char *buffer)
{
    for (int i = 2; buffer[i]; i++)
    {
        if (!((buffer[i] >= '0' && buffer[i] <= '9') || (buffer[i] >= 'a' && buffer[i] <= 'f') || (buffer[i] >= 'A' && buffer[i] <= 'F')))
            return 0;
    }
    return 1;
}
int isitoct(char *buffer)
{
    for (int i = 0; buffer[i]; i++)
    {
        if (!(buffer[i] >= '0' && buffer[i] <= '7'))
            return 0;
    }
    return 1;
}
int isitbin(char *buffer)
{
    for (int i = 0; buffer[i]; i++)
    {
        if (!(buffer[i] == '1' || buffer[i] == '0' || buffer[i] == 'B' || buffer[i] == 'b'))
            return 0;
    }
    return 1;
}
int isitdec(char *buffer)
{
    for (int i = 0; buffer[i]; i++)
    {
        if (!(isdigit(buffer[i])))
        {
            return 0;
        }
    }
    return 1;
}
int isitcl(char *buffer)
{
    if (buffer[3] == '\0')
        return 1;
    else
        return 0;
}
