#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char keywords[32][20] =
{
    "auto","break","case","char","const","continue",
    "default","do","double","else","enum","extern",
    "float","for","goto","if","int","long",
    "register","return","short","signed","sizeof","static",
    "struct","switch","typedef","union","unsigned","void",
    "volatile","while"
};

int keyword_count = 32;

int is_keyword(char *word)
{
    int i;

    for(i = 0; i < keyword_count; i++)
    {
        if(strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

char operators[] = "+-*/%=";

int is_operator(char ch)
{
    int i;

    for(i = 0; operators[i] != '\0'; i++)
    {
        if(ch == operators[i])
        {
            return 1;
        }
    }

    return 0;
}

char symbols[] = ";{}()[],";

int is_specialsymbol(char ch)
{
    int i;

    for(i = 0; symbols[i] != '\0'; i++)
    {
        if(ch == symbols[i])
        {
            return 1;
        }
    }

    return 0;
}

int is_number(char word[])
{
    int i;

    for(i = 0; word[i] != '\0'; i++)
    {
        if(!isdigit(word[i]))
        {
            return 0;
        }
    }

    return 1;
}

int isidentifier(char word[])
{
    int i;

    // first character
    if(!(isalpha(word[0]) || word[0] == '_'))
    {
        return 0;
    }

    // remaining characters
    for(i = 1; word[i] != '\0'; i++)
    {
        if(!(isalnum(word[i]) || word[i] == '_'))
        {
            return 0;
        }
    }

    return 1;
}

void lexicalAnalyzer(FILE *fp)
{
    int ch;
    char buffer[256];
    int i;

    printf("Tokens Found:\n\n");

    while((ch = fgetc(fp)) != EOF)
    {
        // Skip spaces
        if(isspace(ch))
        {
            continue;
        }

        // Ignore single line comments //
        if(ch == '/')
        {
            int next = fgetc(fp);

            // check for //
            if(next == '/')
            {
                // skip complete line
                while((ch = fgetc(fp)) != '\n' && ch != EOF);

                continue;
            }
            else
            {
                // put character back
                ungetc(next, fp);
            }
        }

        // Preprocessor directive
        if(ch == '#')
        {
            i = 0;

            buffer[i++] = ch;

            while((ch = fgetc(fp)) != EOF && !isspace(ch))
            {
                buffer[i++] = ch;
            }

            buffer[i] = '\0';

            if(ch != EOF)
            {
                ungetc(ch, fp);
            }

            printf("%s -> Preprocessor Directive\n", buffer);

            continue;
        }

        // Operator
        if(is_operator(ch))
        {
            printf("%c -> Operator\n", ch);
        }

        // Special Symbol
        else if(is_specialsymbol(ch))
        {
            printf("%c -> Symbol\n", ch);
        }

        // Identifier / Keyword / Number
        else if(isalpha(ch) || isdigit(ch) || ch == '_')
        {
            i = 0;

            buffer[i++] = ch;

            while((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_'))
            {
                buffer[i++] = ch;
            }

            buffer[i] = '\0';

            if(ch != EOF)
            {
                ungetc(ch, fp);
            }

            if(is_keyword(buffer))
            {
                printf("%s -> Keyword\n", buffer);
            }
            else if(is_number(buffer))
            {
                printf("%s -> Number\n", buffer);
            }
            else if(isidentifier(buffer))
            {
                printf("%s -> Identifier\n", buffer);
            }
            else
            {
                printf("%s -> special symbol\n", buffer);
            }
        }

        else
        {
            printf("%c -> special character\n", ch);
        }
    }
}