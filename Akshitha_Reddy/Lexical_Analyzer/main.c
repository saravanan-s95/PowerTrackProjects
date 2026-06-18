/*
NAME		: G AKSHITHA REDDY
DATE		: 11 JUNE, 2026
PROJECT 	: LEXICAL ANALYZER
Description : Reads a C source file, identifies tokens,
              classifies them and performs basic syntax
              checking for parentheses and braces.
*/

#include<stdio.h>
#include<ctype.h>
#include "lexer.h"

int main()
{
    FILE *fptr;
    char ch;
    char token[100];
    int i = 0;
    /* Counters for syntax checking */
    int openParen = 0, closeParen = 0;
    int openBrace = 0, closeBrace = 0;

    /* Open source file in read mode */
    fptr = fopen("text.c", "r");

    if(fptr == NULL)
    {
        printf("File not found!\n");
        return 1;
    }

    printf("\nOUTPUT\n\n");

    /* Read source file character by character */
    while((ch = fgetc(fptr)) != EOF)
    {
        /* Count opening and closing parentheses */
        if(ch == '(')
        openParen++;

        if(ch == ')')
        closeParen++;

        /* Count opening and closing braces */
        if(ch == '{')
        openBrace++;

        if(ch == '}')
        closeBrace++;

         /* Build token using letters, digits, '_' and '.' */
        if(isalnum(ch) || ch == '_' || ch == '.')
        {
            token[i++] = ch;
        }
        else
        {
            if(i != 0)
            {
                token[i] = '\0';
                /* Classify token into its corresponding category */
                if(isDataKeyword(token))
                {
                    printf("Reserved Keyword(Data Type)      : %s\n", token);
                }
                else if(isnonDataKeyword(token))
                {
                    printf("Reserved Keyword(Non Data Type)  : %s\n", token);
                }
                else if(isInteger(token))
                {
                    printf("Integer Literal                  : %s\n", token);
                }
                else if(isFloat(token))
                {
                    printf("Float Literal                    : %s\n", token);
                }
                else
                {
                    printf("Identifier                       : %s\n", token);
                }

                i = 0;
            }
            /* Check whether current character is an operator */
            if(isOperator(ch))
            {
                printf("Operator                         : %c\n", ch);
            }
            /* Check whether current character is an special character */
            else if(isSpclChar(ch))
            {
                printf("Special Character                : %c\n", ch);
            }
        }
    }
     /* Verify matching parentheses */
    if(openParen > closeParen)
    {
        printf("\nERROR: Missing ')'\n");
    }
    else if(closeParen > openParen)
    {
        printf("\nERROR: Extra ')'\n");
    }
     /* Verify matching braces */
    if(openBrace > closeBrace)
    {
        printf("ERROR: Missing '}'\n");
    }
    else if(closeBrace > openBrace)
    {
        printf("ERROR: Extra '}'\n");
    }
    /* Close the file */
    fclose(fptr);

    return 0;
}
	
