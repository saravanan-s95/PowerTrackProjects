/* error.c */

#include "lexicalanalyser.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// Invalid identifier
void err_identifier(int line, int col, char *word)
{
    printf("error.c:%d:%d: error: invalid identifier '%s'\n",
           line, col, word);

    exit(0);
}

// Integer validation
int err_integer(int line, int col, char *word)
{
    int i;

    for (i = 0; word[i] != '\0'; i++)
    {
        if (!isdigit(word[i]))
        {
            printf("error.c:%d:%d: error: invalid integer constant '%s'\n",
                   line, col, word);

            exit(0);
        }
    }

    return 1;
}

// Float validation
int err_float(int line, int col, char *word)
{
    int i;
    int dot = 0;

    for (i = 0; word[i] != '\0'; i++)
    {
        if (word[i] == '.')
        {
            dot++;

            if (dot > 1)
            {
                printf("error.c:%d:%d: error: invalid float constant '%s'\n",
                       line, col, word);

                exit(0);
            }
        }
        else if (!isdigit(word[i]))
        {
            printf("error.c:%d:%d: error: invalid suffix '%c' on floating constant\n",
                   line, col + i, word[i]);

            exit(0);
        }
    }

    return 1;
}

// Parenthesis check
void err_parenthesis(int line, int col, char buffer[], int i)
{
    int count = 1;

    i++;

    while (buffer[i] != '\0')
    {
        if (buffer[i] == '(')
        {
            count++;
        }
        else if (buffer[i] == ')')
        {
            count--;
        }

        if (count == 0)
        {
            return;
        }

        i++;
    }

    printf("error.c:%d:%d: error: expected ')' before end of input\n",
           line, col);

    exit(0);
}

// Brace check
void err_brace(int line, int col, char buffer[], int i)
{
    int count = 1;

    i++;

    while (buffer[i] != '\0')
    {
        if (buffer[i] == '{')
        {
            count++;
        }
        else if (buffer[i] == '}')
        {
            count--;
        }

        if (count == 0)
        {
            return;
        }

        i++;
    }

    printf("error.c:%d:%d: error: expected '}' before end of input\n",
           line, col);

    exit(0);
}

// Bracket check
void err_bracket(int line, int col, char buffer[], int i)
{
    int count = 1;

    i++;

    while (buffer[i] != '\0')
    {
        if (buffer[i] == '[')
        {
            count++;
        }
        else if (buffer[i] == ']')
        {
            count--;
        }

        if (count == 0)
        {
            return;
        }

        i++;
    }

    printf("error.c:%d:%d: error: expected ']' before end of input\n",
           line, col);

    exit(0);
}