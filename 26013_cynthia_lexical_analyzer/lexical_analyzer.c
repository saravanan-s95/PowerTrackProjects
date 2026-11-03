
#include "lexical_analyzer.h"
#include "functions.h"
#include <ctype.h>

void lexical_analyze(FILE *fp)
{
    int ch;
    char token[100];
    int i;

    while ((ch = fgetc(fp)) != EOF)
    {
        if (isalpha(ch) || ch == '_')
        {
            i = 0;
            token[i++] = ch;

            while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_'))
            {
                if (i < 99)
                {
                    token[i++] = ch;
                }
            }

            token[i] = '\0';
            if (is_keyword(token) == SUCCESS)
            {
                printf("%-10s : %s\n", "Keyword", token);
            }
            else
            {
                printf("%-10s : %s\n", "Identifier", token);
            }

            if (ch == EOF)
            {
                break;
            }

            ungetc(ch, fp);
        }
        
        else if (is_digit(ch) == SUCCESS)
        {
            i = 0;
            token[i++] = ch;

            if (ch == '0')
            {
                ch = fgetc(fp);

                if (ch == 'x' || ch == 'X')
                {
                    token[i++] = ch;

                    while ((ch = fgetc(fp)) != EOF &&
                        ((ch >= '0' && ch <= '9') ||
                            (ch >= 'a' && ch <= 'f') ||
                            (ch >= 'A' && ch <= 'F')))
                    {
                        if (i < 99)
                        {
                            token[i++] = ch;
                        }
                    }
                }
                else
                {
                    while (ch != EOF && is_digit(ch) == SUCCESS)
                    {
                        if (i < 99)
                        {
                            token[i++] = ch;
                        }

                        ch = fgetc(fp);
                    }
                }
            }
            else
            {
                while ((ch = fgetc(fp)) != EOF && is_digit(ch) == SUCCESS)
                {
                    if (i < 99)
                    {
                        token[i++] = ch;
                    }
                }
            }

            token[i] = '\0';

            if (is_hexa(token) == SUCCESS)
            {
                printf("%-10s : %s\n", "Literal", token);
            }
            else if (is_octal(token) == SUCCESS)
            {
                printf("%-10s : %s\n", "Literal", token);
            }
            else
            {
                printf("%-10s : %s\n", "Literal", token);
            }

            if (ch == EOF)
            {
                break;
            }

            ungetc(ch, fp);
        }
        else if (ch == '"' || ch == '\'')
        {
            int delimiter = ch;

            i = 0;
            token[i++] = ch;

            while ((ch = fgetc(fp)) != EOF && ch != delimiter)
            {
                if (i < 99)
                {
                    token[i++] = ch;
                }
            }

            if (ch == delimiter && i < 99)
            {
                token[i++] = ch;
            }

            token[i] = '\0';

            printf("%-10s : %s\n", "Literal", token);
        }

        else if (ch == '/')
        {
            int next;

            next = fgetc(fp);

            if (next == '/')
            {
                while ((ch = fgetc(fp)) != EOF && ch != '\n')
                {
                    ;
                }
            }
            else if (next == '*')
            {
                int prev = 0;

                while ((ch = fgetc(fp)) != EOF)
                {
                    if (prev == '*' && ch == '/')
                    {
                        break;
                    }

                    prev = ch;
                }
            }
            else
            {
                if (next != EOF)
                {
                    ungetc(next, fp);
                }

                printf("Operator :/\n");
            }
        }

        else if (is_assignment(ch) == SUCCESS)
        {
            int next;

            next = fgetc(fp);

            if (next == '=')
            {
                printf("%-10s : %c\n", "Operator", ch);
            }
            else
            {
                printf("%-10s : %c\n", "Operator", ch);

                if (next != EOF)
                {
                    ungetc(next, fp);
                }
            }
        }
                
            
        else if (is_operator(ch) == SUCCESS)
        {
            int next;

            next = fgetc(fp);

            if ((ch == '&' && next == '&') ||
                (ch == '|' && next == '|') ||
                (ch == '+' && (next == '+' || next == '=')) ||
                (ch == '-' && (next == '-' || next == '=')) ||
                (ch == '*' && next == '=') ||
                (ch == '/' && next == '=') ||
                (ch == '%' && next == '=') ||
                (ch == '=' && next == '=') ||
                (ch == '!' && next == '=') ||
                (ch == '<' && (next == '=' || next == '<')) ||
                (ch == '>' && (next == '=' || next == '>')))
            {
                printf("%-10s : %c%c\n", "Operator", ch, next);
            }
            else
            {
                printf("%-10s : %c\n", "Operator", ch);
                

                if (next != EOF)
                {
                    ungetc(next, fp);
                }
            }
        }
        else if (isspace(ch))
        {
            continue;
        }
        


    }
}
