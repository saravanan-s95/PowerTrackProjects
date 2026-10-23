#include <stdio.h>
#include <string.h>
#include "analyzer.h"

// Check whether a word is a C keyword
bool isKeyword(const char *word)
{
    const char *keywords[] = {
        "auto", "break", "case", "char", "const",
        "continue", "default", "do", "double", "else",
        "enum", "extern", "float", "for", "goto",
        "if", "inline", "int", "long", "register",
        "restrict", "return", "short", "signed", "sizeof",
        "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while",
        "_Bool", "_Complex", "_Imaginary"
    };

    int count = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return true;
        }
    }

    return false;
}

// Print the token type as readable text
void printToken(Token token)
{
    const char *typeName;

    switch (token.type)
    {
        case TOKEN_KEYWORD:
            typeName = "KEYWORD";
            break;

        case TOKEN_IDENTIFIER:
            typeName = "IDENTIFIER";
            break;

        case TOKEN_INTEGER:
            typeName = "INTEGER";
            break;

        case TOKEN_FLOAT:
            typeName = "FLOAT";
            break;

        case TOKEN_OPERATOR:
            typeName = "OPERATOR";
            break;

        case TOKEN_SPECIAL_SYMBOL:
            typeName = "SPECIAL SYMBOL";
            break;

        case TOKEN_STRING:
            typeName = "STRING";
            break;

        case TOKEN_CHARACTER:
            typeName = "CHARACTER";
            break;

        case TOKEN_EOF:
            typeName = "EOF";
            break;

        case TOKEN_ERROR:
            typeName = "ERROR";
            break;

        default:
            typeName = "UNKNOWN";
            break;
    }

    printf("Line %-3d | %-15s | %s\n",
           token.line, typeName, token.lexeme);
}