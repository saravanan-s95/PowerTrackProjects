#include "lexer.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int read_fun(char *filename)
{
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("Open failed\n");
        return 1;
    }

    char ch;
    char symbols[] = {'[', ']', '(', ')', '{', '}', ';', '!', '@', '$', ','};
    char operators[] = {'+', '-', '*', '/', '>', '<', '=', '!', '%', '~'};

    int line = 1;

    // Counters for unclosed brackets/quotes
    int curly = 0, square = 0, round = 0;

    while ((ch = fgetc(fptr)) != EOF)
    {
        if (ch == '\n')
        {
            line++;
        }
        if (ch == '#') // Skip preprocessor lines
        {
            printf("Preprocessor     : #");
            while ((ch = fgetc(fptr)) != '\n' && ch != EOF)
            {
                printf("%c", ch);
            }
            printf("\n");
            if (ch == '\n')
            {
                line++;
            }
        }

        char buffer[50]; // increased size for safety

        // Track brackets
        if (ch == '{')
            curly++;
        else if (ch == '}')
        {
            curly--;
            if (curly < 0)
            {
                printf("ERROR: Extra '}' \n");
                curly = 0;
                continue;
            }
        }

        if (ch == '[')
            square++;
        else if (ch == ']')
        {

            square--;
            if (square < 0)
            {
                printf("ERROR: Extra ']' at line %d\n", line);
                square = 0;
                continue;
            }
        }

        if (ch == '(')
            round++;
        else if (ch == ')')
        {
            round--;
            if (round < 0)
            {
                printf("ERROR: Extra ')' at line %d\n", line);
                round = 0;
                continue;
            }
        }

        // 1️ Identifier or Keyword
        if (isalpha(ch) || ch == '_')
        {
            int i = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fptr)) != EOF && (isalnum(ch) || ch == '_'))
            {
                buffer[i++] = ch;
                if (ch == '\n')
                    line++;
            }
            buffer[i] = '\0';
            ungetc(ch, fptr);

            if (keyword(buffer))
                printf("Keyword          : %s\n", buffer);
            else
            {
                if (ident(buffer))
                    printf("Identifier       : %s\n", buffer);
            }
        }
        // 2Number literals
        else if (isdigit(ch))
        {
            int i = 0;
            buffer[i++] = ch;

            if (ch != '0') // Decimal or Float
            {
                int isFloat = 0;
                int letter = 0;
                while ((ch = fgetc(fptr)) != EOF && (isalnum(ch) || ch == '.'))
                {
                    if (ch == '.')
                        isFloat = 1;
                    buffer[i++] = ch;
                    if (isalpha(ch))
                        letter = 1;
                    // if(ch=='\n') line++;
                }
                buffer[i] = '\0';
                ungetc(ch, fptr);

                if (isFloat)
                {
                    if (isitfloat(buffer))
                        printf("Float Literal    : %s\n", buffer);
                    else
                    {
                        printf("ERROR AT LINE %d: INVALID FLOAT LITERAL !!!\n", line);
                        return 1;
                    }
                }

                else if (letter)
                {
                    printf("ERROR AT LINE %d: INVALID IDENTIFIER !!!\n", line);
                    return 1;
                }
                else
                {
                    if (isitdec(buffer))
                        printf("Decimal Literal  : %s\n", buffer);
                    else
                    {
                        printf("ERROR AT LINE %d: INVALID DECIMAL LITERAL !!!\n", line);
                        return 1;
                    }
                }
            }
            else // Starts with 0 → Octal, Hex, Binary, or decimal
            {
                ch = fgetc(fptr);
                if (ch == '.') // Float like 0.133
                {
                    buffer[i++] = ch;
                    while ((ch = fgetc(fptr)) != EOF && isalnum(ch))
                    {
                        buffer[i++] = ch;
                        // if(ch == '\n') line++;
                    }
                    buffer[i] = '\0';
                    ungetc(ch, fptr);
                    if (isitfloat(buffer))
                        printf("Float Literal    : %s\n", buffer);
                    else
                    {
                        printf("ERROR AT LINE %d: INVALID FLOAT LITERAL !!!\n", line);
                        return 1;
                    }
                }
                else if (ch == 'x' || ch == 'X') // Hexadecimal
                {
                    buffer[i++] = ch;
                    while ((ch = fgetc(fptr)) != EOF && isalnum(ch))
                    {
                        buffer[i++] = ch;
                        if (ch == '\n')
                            line++;
                    }
                    buffer[i] = '\0';
                    ungetc(ch, fptr);

                    if (isithexa(buffer))
                        printf("Hexa Literal     : %s\n", buffer);
                    else
                    {
                        printf("ERROR AT LINE %d: INVALID HEXA LITERAL  !!!\n", line);
                        return 1;
                    }
                }
                else if (ch == 'b' || ch == 'B') // Binary
                {
                    buffer[i++] = ch;
                    while ((ch = fgetc(fptr)) != EOF && isalnum(ch))
                    {
                        buffer[i++] = ch;
                        if (ch == '\n')
                            line++;
                    }
                    buffer[i] = '\0';
                    ungetc(ch, fptr);
                    if (isitbin(buffer))
                        printf("Binary Literal   : %s\n", buffer);
                    else
                    {
                        printf("ERROR AT LINE %d: INVALID BINARY LITERAL !!!\n", line);
                        return 1;
                    }
                }
                else if (ch >= '0' && ch <= '7') // Octal
                {
                    buffer[i++] = ch;
                    while ((ch = fgetc(fptr)) != EOF && isdigit(ch))
                    {
                        buffer[i++] = ch;
                        if (ch == '\n')
                            line++;
                    }
                    buffer[i] = '\0';
                    ungetc(ch, fptr);
                    if (isitoct(buffer))
                        printf("Octal Literal    : %s\n", buffer);
                    else
                    {
                        printf("ERROR AT LINE %d: INVALID OCTAL LITERAL !!!\n", line);
                        return 1;
                    }
                }
                else // Just 0
                {
                    buffer[i] = '\0';
                    ungetc(ch, fptr);
                    printf("Decimal Literal  : %s\n", buffer);
                }
            }
        }

        // 3) Char literal
        else if (ch == '\'')
        {
            int s_line = line;
            int i = 0, closed = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fptr)) != EOF)
            {
                buffer[i++] = ch;
                if (ch == '\'')
                {
                    closed = 1;
                    break;
                }
                if (ch == '\n')
                    line++;
            }
            buffer[i] = '\0';
            if (closed && isitcl(buffer))
                printf("Char Literal     : %s\n", buffer);
            else if (!closed)
            {
                printf("ERROR : UNCLOSED CHAR LITERAL at %d!!!\n", s_line);

                return 1;
            }
            else
            {
                printf("ERROR AT LINE %d: INVALID CHAR LITERAL !!!\n", line);
                return 1;
            }
        }
        // 4) String literal
        else if (ch == '"')
        {
            int s_line = line;
            int i = 0, closed = 0;
            buffer[i++] = ch;
            while ((ch = fgetc(fptr)) != EOF)
            {
                buffer[i++] = ch;
                if (ch == '"')
                {
                    closed = 1;
                    break;
                }
                if (ch == '\n')
                    line++;
            }
            buffer[i] = '\0';
            if (closed)
                printf("String Literal   : %s\n", buffer);
            else
            {
                printf("ERROR: Unclosed string literal line %d !!!\n", s_line);

                return 1;
            }
        }
        // 5)Special Characters and Operators
        else
        {
            int isSymbol = 0;

            // Special characters
            for (int i = 0; i < sizeof(symbols) / sizeof(symbols[0]); i++)
            {
                if (ch == symbols[i])
                {
                    printf("S-Character      : %c\n", ch);
                    isSymbol = 1;
                    break;
                }
            }

            // Operators
            if (!isSymbol)
            {
                for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++)
                {
                    if (ch == operators[i])
                    {
                        char next = fgetc(fptr);
                        if ((ch == '=' && next == '=') ||
                            (ch == '!' && next == '=') ||
                            (ch == '<' && next == '=') ||
                            (ch == '>' && next == '=') ||
                            (ch == '&' && next == '&') ||
                            (ch == '|' && next == '|'))
                        {
                            printf("Operator         : %c%c\n", ch, next);
                        }
                        else
                        {
                            ungetc(next, fptr); // push back if not 2-char operator
                            printf("Operator         : %c\n", ch);
                        }
                        break;
                    }
                }
            }
        }
    }

    // Print errors for unclosed brackets/quotes
    if (curly > 0)
        printf("ERROR: Unclosed '{'\n");
    if (square > 0)
        printf("ERROR: Unclosed '['\n");
    if (round > 0)
        printf("ERROR: Unclosed '('\n");

    fclose(fptr);
    return 0;
}
