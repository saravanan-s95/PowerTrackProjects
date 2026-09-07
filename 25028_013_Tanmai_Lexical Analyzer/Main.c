#include "Main.h"

int main()
{
    FILE *fptr;              // File pointer to read source file
    char ch;                 // Stores each character read from file
    char token[100];         // Stores extracted token
    int i = 0;               // Token index
    /* Counters for syntax validation */
    int openParen = 0, closeParen = 0;
    int openBrace = 0, closeBrace = 0;
    /* Open source file in read mode */
    fptr = fopen("code.c", "r");
     /* Check whether file opened successfully */
    if(fptr == NULL)
    {
        printf("File not found\n");
        return 1;
    }

    printf("\n----- LEXICAL ANALYZER OUTPUT -----\n\n");
    /* Read file character by character */

    while((ch = fgetc(fptr)) != EOF)
    {
        /* Count opening parenthesis */
        if(ch == '(')
            openParen++;
        /* Count closing parenthesis */
        if(ch == ')')
            closeParen++;
        /* Count opening braces */
        if(ch == '{')
            openBrace++;
          /* Count closing braces */
        if(ch == '}')
            closeBrace++;
         /* Build token if character is alphanumeric, underscore or decimal point */
        if(isalnum(ch) || ch == '_' || ch == '.')
        {
            token[i++] = ch;
        }
        else
        {
            /* End of token reached */
            if(i != 0)
            {
                token[i] = '\0';
                /* Check whether token is a data type keyword */
                if(isDataKeyword(token))
                {
                    printf("Data Type Keyword      : %s\n", token);
                }
                /* Check whether token is a non-data keyword */
                else if(isNonDataKeyword(token))
                {
                    printf("Non Data Keyword       : %s\n", token);
                }
                 /* Check whether token is an integer literal */
                else if(isInteger(token))
                {
                    printf("Integer Literal        : %s\n", token);
                }
                /* Check whether token is a floating-point literal */
                else if(isFloat(token))
                {
                    printf("Float Literal          : %s\n", token);
                }
                 /* Otherwise classify as identifier */
                else
                {
                    printf("Identifier             : %s\n", token);
                }
                /* Reset token index for next token */
                i = 0;
            }
            /* Check for operators */
            if(isOperator(ch))
            {
                printf("Operator               : %c\n", ch);
            }
             /* Check for special characters */
            else if(isSpecialChar(ch))
            {
                printf("Special Character      : %c\n", ch);
            }
        }
    }

    printf("\n");
    /* Check for missing or extra parentheses */
    if(openParen > closeParen)
        printf("ERROR : Missing )\n");

    else if(closeParen > openParen)
        printf("ERROR : Extra )\n");
    /* Check for missing or extra braces */
    if(openBrace > closeBrace)
        printf("ERROR : Missing }\n");

    else if(closeBrace > openBrace)
        printf("ERROR : Extra }\n");
    /* Close the file */
    fclose(fptr);

    return 0;
}