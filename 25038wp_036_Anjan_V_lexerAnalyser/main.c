/*

    Name : Anjana V
    Description: Lexial Analyser -
                    Keywords (int, if, while, etc.)
                    Identifiers (main, count, etc.)
                    Numbers (10, 3.14)
                    Operators (+, -, =, etc.)
                    Special symbols ({, }, ;, etc.)
*/

#include <stdio.h>
#include "main.h"


int main(int argc, char *argv[])
{
    if(argc == 2)
    {
        FILE *fp;
        char ch;
        char token[100];
        int i;
        // open sample file in read mode
        fp = fopen(argv[1], "r");
        // checks file operation failed
        if(fp == NULL)
        {
            printf("Cannot open file: %s\n", argv[1]);
            return 0;
        }
        printf("\nTOKEN\t\tTYPE\n");
        printf("--------------------------\n");

        while((ch = fgetc(fp)) != EOF)
        {
            // ignore whitespace
            if(isspace(ch))
                continue;

            // checks Keyword / Identifier
            if(isalpha(ch) || ch == '_')
            {
                i = 0;
                // stores char
                token[i++] = ch;
                // reading remaing char
                // identifier can contain: alphabets (a-z, A-Z), digits (0-9), underscore (_)
                while((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_'))
                {
                    token[i++] = ch;
                }
                // making the token a string
                token[i] = '\0';
                // if keyword
                if(isKeyword(token))
                {
                    printf("%-15s Keyword\n", token);
                }
                // if identifier
                else
                {
                    printf("%-15s Identifier\n", token);
                }
                // put back non number char
                if(ch != EOF)
                {
                    ungetc(ch, fp);
                }
            }

            /* chicking if number */
            else if(isdigit(ch))
            {
                i = 0;
                token[i++] = ch;
                // reading remaining digit or decimal
                while((ch = fgetc(fp)) != EOF && (isdigit(ch) || ch == '.'))
                {
                    token[i++] = ch;
                }
                // adding string terminator
                token[i] = '\0';
                printf("%-15s Number\n", token);
                // put back non number char
                if(ch != EOF)
                {
                    ungetc(ch, fp);
                }
            }

            /* operator */
            else if(isOperator(ch))
            {
                printf("%-15c Operator\n", ch);
            }

            /* special character */
            else if(isSpecialCharacter(ch))
            {
                printf("%-15c Special Symbol\n", ch);
            }
            /* unknown */
            else
            {
                printf("%-15c Unknown\n", ch);
            }
        }
        // close the file
        fclose(fp);

    }
    // invalid command line usage
    else
    {
        printf("Invalid arguments : ./a.out <input.c>\n");
    }
    return 0;
}