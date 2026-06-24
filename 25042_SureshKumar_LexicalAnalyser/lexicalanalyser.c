/* lexicalanalyser.c */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexicalanalyser.h"

extern char buffer[5000];

// Keyword array
const char *keywords[] =
{
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof",
    "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while"
};

// Number of keywords
int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

// Keyword checking function
int isKeyword(char *word)
{
    for (int i = 0; i < keyword_count; i++)
    {
        if (!strcmp(word, keywords[i]))
        {
            return 1;
        }
    }

    return 0;
}

void lexicalanalyser()
{
    int line = 1, col = 0;

    char word[1000];

    int k = 0;

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        char ch = buffer[i];

        col++;

        // New line CHECKING
        if (ch == '\n')
        {
            line++;
            col = 0;
            continue;
        }

        // Skip spaces and tabs
        if (ch == ' ' || ch == '\t')
        {
            continue;
        }

        // Preprocessor directives
        if (ch == '#')
        {
            while (buffer[i] != '\n' &&
                   buffer[i] != '\0')
            {
                i++;
                col++;
            }

            i--;//TO AVOID OVERFLOW

            continue;
        }

        // Single line comments
        if (ch == '/' && buffer[i + 1] == '/')
        {
            i += 2;
            col += 2;

            while (buffer[i] != '\n' &&
                   buffer[i] != '\0')
            {
                i++;
                col++;
            }

            i--;

            continue;
        }

        // Multi line comments
        if (ch == '/' && buffer[i + 1] == '*')
        {
            i += 2;
            col += 2;

            while (!(buffer[i] == '*' &&
                     buffer[i + 1] == '/'))
            {
                if (buffer[i] == '\0')
                {
                    printf("error.c:%d:%d: error: unterminated comment\n",
                           line, col);

                    exit(0);
                }

                if (buffer[i] == '\n')
                {
                    line++; 
                    col = 0;
                }
                else
                {
                    col++;
                }

                i++;
            }

            // Skip */
            i += 1;  //for pointing / so outer loop increments it
            col += 1;

            continue;
        }

        // String literal
        if (ch == '"')
        {
            k = 0;

            word[k++] = ch;

            i++;
            col++;

            while (buffer[i] != '"' &&
                   buffer[i] != '\n' &&
                   buffer[i] != '\0')  //before colsing " new line,null must not be present and we storing characters before closing "
            {
                if (k < sizeof(word) - 1)
                {
                    word[k++] = buffer[i];
                }

                i++;
                col++;
            }

            if (buffer[i] == '"')
            {
                if (k < sizeof(word) - 1)
                {
                    word[k++] = buffer[i];
                }

                word[k] = '\0';

                printf("String Literal : %s\n", word);

                continue;
            }
            else
            {
                printf("error.c:%d:%d: error: missing terminating '\"' character\n",
                       line, col);

                exit(0);
            }
        }

        // Character literal
        if (ch == '\'') //to detect character literals like 'a'
        {
            k = 0;

            word[k++] = ch;

            i++;
            col++;

            // Escape sequence
            if (buffer[i] == '\\') /*Examples of valid character literals 'a' '7' '#' '\n' '\t' '\\' '\''*/
            {
                if (k < sizeof(word) - 1)
                {
                    word[k++] = buffer[i];
                }

                i++;
                col++;
            }

            // Actual character
            if (buffer[i] != '\0' &&
                buffer[i] != '\n')
            {
                if (k < sizeof(word) - 1)
                {
                    word[k++] = buffer[i];
                }

                i++;
                col++;
            }

            // Closing '
            if (buffer[i] == '\'')
            {
                if (k < sizeof(word) - 1)
                {
                    word[k++] = buffer[i];
                }

                word[k] = '\0';

                printf("Character Literal : %s\n", word);

                continue;
            }
            else
            {
                printf("error.c:%d:%d: error: missing terminating '\'' character\n",
                       line, col);

                exit(0);
            }
        }

        // Identifier / Keyword
        if (isalpha(ch) || ch == '_') //isalpha checks it is character from a-z or A-Z
        {
            k = 0;

            while (isalnum(buffer[i]) || buffer[i] == '_') //isalnum checks both char and digit and we also checks it starts with underscore
            {
                if (k < sizeof(word) - 1)
                {
                    word[k++] = buffer[i];
                }

                i++;
                col++;
            }

            word[k] = '\0';

            i--;
            col--;

            // Keyword check using array
            if (isKeyword(word))
            {
                printf("Keyword : %s\n", word);
            }
            else
            {
                printf("Identifier : %s\n", word);
            }

            continue;
        }

        // Number detection
        if (isdigit(ch))
        {
            k = 0;

            int dot = 0;

            while (isdigit(buffer[i]) || buffer[i] == '.')
            {
                if (buffer[i] == '.')
                {
                    dot++;
                }

                if (k < sizeof(word) - 1)
                {
                    word[k++] = buffer[i];
                }

                i++;
                col++;
            }

            // Invalid identifier like 12abc
            if (isalpha(buffer[i]) || buffer[i] == '_')
            {
                while (isalnum(buffer[i]) || buffer[i] == '_')
                {
                    if (k < sizeof(word) - 1)
                    {
                        word[k++] = buffer[i];
                    }

                    i++;
                    col++;
                }

                word[k] = '\0';

                err_identifier(line, col, word);
            }

            word[k] = '\0';

            i--;
            col--;

            // Float constant
            if (dot)
            {
                err_float(line, col, word);

                printf("Float Constant : %s\n", word);
            }
            else
            {
                err_integer(line, col, word);

                printf("Integer Constant : %s\n", word);
            }

            continue;
        }

        // Operators
        if (ch == '+' || ch == '-' ||
            ch == '*' || ch == '/' ||
            ch == '=' || ch == '<' ||
            ch == '>' || ch == '!' ||
            ch == '&' || ch == '|')
        {
            // Double operators
            if ((ch == '+' && buffer[i + 1] == '+') ||
                (ch == '-' && buffer[i + 1] == '-') ||
                (ch == '=' && buffer[i + 1] == '=') ||
                (ch == '!' && buffer[i + 1] == '=') ||
                (ch == '<' && buffer[i + 1] == '=') ||
                (ch == '>' && buffer[i + 1] == '=') ||
                (ch == '&' && buffer[i + 1] == '&') ||
                (ch == '|' && buffer[i + 1] == '|'))
            {
                printf("Double Operator : %c%c\n",
                       ch, buffer[i + 1]);

                i++;
                col++;
            }
            else
            {
                printf("Operator : %c\n", ch);
            }

            continue;
        }

        // Symbols
        if (ch == '(' || ch == ')' ||
            ch == '{' || ch == '}' ||
            ch == '[' || ch == ']' ||
            ch == ';' || ch == ',')
        {
            if (ch == '(')
            {
                err_parenthesis(line, col, buffer, i);
            }

            if (ch == '{')
            {
                err_brace(line, col, buffer, i);
            }

            if (ch == '[')
            {
                err_bracket(line, col, buffer, i);
            }

            printf("Symbol : %c\n", ch);

            continue;
        }

        // Unknown token
        printf("error.c:%d:%d: error: unknown token '%c'\n",
               line, col, ch);

        exit(0);
    }
}