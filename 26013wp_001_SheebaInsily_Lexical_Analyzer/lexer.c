#include "lexer.h"

/* C keywords */
const char *keywords[] =
{
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "int",
    "long",
    "register",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while"
};

#define KEYWORD_COUNT (sizeof(keywords) / sizeof(keywords[0]))

/* Check whether a word is a C keyword */
int is_keyword(const char *word)
{
    size_t i;

    for (i = 0; i < KEYWORD_COUNT; i++)
    {
        if (strcmp(word, keywords[i]) == 0)
        {
            return 1;
        }
    }

    return 0;
}

/* Check whether a character is a special character */
int is_special_character(char ch)
{
    switch (ch)
    {
        case '(':
        case ')':
        case '{':
        case '}':
        case '[':
        case ']':
        case ';':
        case ',':
        case ':':
        case '.':
        case '?':
            return 1;

        default:
            return 0;
    }
}

/* Check whether a character can start an identifier */
int is_identifier_start(char ch)
{
    return isalpha((unsigned char)ch) || ch == '_';
}

/* Check whether a character can be part of an identifier */
int is_identifier_char(char ch)
{
    return isalnum((unsigned char)ch) || ch == '_';
}

/* Check whether a character is an operator */
int is_operator(char ch)
{
    switch (ch)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '=':
        case '<':
        case '>':
        case '!':
        case '&':
        case '|':
        case '^':
        case '~':
            return 1;

        default:
            return 0;
    }
}

/* Print token */
void print_token(TokenType type, const char *value)
{
    switch (type)
    {
        case TOKEN_KEYWORD:
            printf("KEYWORD             : %s\n", value);
            break;

        case TOKEN_IDENTIFIER:
            printf("IDENTIFIER          : %s\n", value);
            break;

        case TOKEN_CONSTANT:
            printf("CONSTANT            : %s\n", value);
            break;

        case TOKEN_OPERATOR:
            printf("OPERATOR            : %s\n", value);
            break;

        case TOKEN_SPECIAL_CHARACTER:
            printf("SPECIAL CHARACTER   : %s\n", value);
            break;

        case TOKEN_STRING:
            printf("STRING              : %s\n", value);
            break;

        case TOKEN_CHARACTER:
            printf("CHARACTER           : %s\n", value);
            break;

        case TOKEN_HEADER:
            printf("HEADER              : %s\n", value);
            break;

        case TOKEN_UNKNOWN:
        default:
            printf("UNKNOWN             : %s\n", value);
            break;
    }
}

/* Start lexical analysis */
void lexical_analyzer(FILE *fp)
{
    char ch;
    char token[MAX_TOKEN_SIZE];
    int i;

    while ((ch = fgetc(fp)) != EOF)
    {
        /* Ignore white spaces */
        if (isspace((unsigned char)ch))
        {
            continue;
        }

        /* Header */
        if (ch == '#')
        {
            i = 0;
            token[i++] = ch;

            while ((ch = fgetc(fp)) != EOF && ch != '\n')
            {
                if (i < MAX_TOKEN_SIZE - 1)
                {
                    token[i++] = ch;
                }
            }

            token[i] = '\0';

            print_token(TOKEN_HEADER, token);
            continue;
        }

        /* Identifier or keyword */
        if (is_identifier_start(ch))
        {
            i = 0;
            token[i++] = ch;

            while ((ch = fgetc(fp)) != EOF &&
                   is_identifier_char(ch))
            {
                if (i < MAX_TOKEN_SIZE - 1)
                {
                    token[i++] = ch;
                }
            }

            token[i] = '\0';

            if (ch != EOF)
            {
                ungetc(ch, fp);
            }

            if (is_keyword(token))
            {
                print_token(TOKEN_KEYWORD, token);
            }
            else
            {
                print_token(TOKEN_IDENTIFIER, token);
            }

            continue;
        }

        /* Integer or floating-point constant */
        if (isdigit((unsigned char)ch))
        {
            i = 0;
            token[i++] = ch;

            while ((ch = fgetc(fp)) != EOF &&
                   (isdigit((unsigned char)ch) || ch == '.'))
            {
                if (i < MAX_TOKEN_SIZE - 1)
                {
                    token[i++] = ch;
                }
            }

            token[i] = '\0';

            if (ch != EOF)
            {
                ungetc(ch, fp);
            }

            print_token(TOKEN_CONSTANT, token);
            continue;
        }

        /* String */
        if (ch == '"')
        {
            i = 0;
            token[i++] = ch;

            while ((ch = fgetc(fp)) != EOF)
            {
                if (i < MAX_TOKEN_SIZE - 1)
                {
                    token[i++] = ch;
                }

                if (ch == '"')
                {
                    break;
                }
            }

            token[i] = '\0';

            print_token(TOKEN_STRING, token);
            continue;
        }

        /* Character */
        if (ch == '\'')
        {
            i = 0;
            token[i++] = ch;

            while ((ch = fgetc(fp)) != EOF)
            {
                if (i < MAX_TOKEN_SIZE - 1)
                {
                    token[i++] = ch;
                }

                if (ch == '\'')
                {
                    break;
                }
            }

            token[i] = '\0';

            print_token(TOKEN_CHARACTER, token);
            continue;
        }

        /* Comments and division operators */
        if (ch == '/')
        {
            char next = fgetc(fp);

            /* Single-line comment */
            if (next == '/')
            {
                while ((ch = fgetc(fp)) != EOF && ch != '\n')
                {
                    /* Skip comment */
                }

                continue;
            }

            /* Multi-line comment */
            if (next == '*')
            {
                char previous = 0;

                while ((ch = fgetc(fp)) != EOF)
                {
                    if (previous == '*' && ch == '/')
                    {
                        break;
                    }

                    previous = ch;
                }

                continue;
            }

            /* Divide assignment operator */
            if (next == '=')
            {
                print_token(TOKEN_OPERATOR, "/=");
                continue;
            }

            /* Division operator */
            if (next != EOF)
            {
                ungetc(next, fp);
            }

            print_token(TOKEN_OPERATOR, "/");
            continue;
        }

        /* Operators */
        if (is_operator(ch))
        {
            char next;
            int two_character_operator = 0;

            token[0] = ch;
            token[1] = '\0';

            next = fgetc(fp);

            /* Two-character operators */
            if ((ch == '=' && next == '=') ||
                (ch == '!' && next == '=') ||
                (ch == '<' && next == '=') ||
                (ch == '>' && next == '=') ||
                (ch == '+' && next == '+') ||
                (ch == '-' && next == '-') ||
                (ch == '&' && next == '&') ||
                (ch == '|' && next == '|') ||
                (ch == '+' && next == '=') ||
                (ch == '-' && next == '=') ||
                (ch == '*' && next == '=') ||
                (ch == '%' && next == '='))
            {
                token[1] = next;
                token[2] = '\0';
                two_character_operator = 1;
            }

            if (!two_character_operator && next != EOF)
            {
                ungetc(next, fp);
            }

            print_token(TOKEN_OPERATOR, token);
            continue;
        }

        /* Special characters */
        if (is_special_character(ch))
        {
            token[0] = ch;
            token[1] = '\0';

            print_token(TOKEN_SPECIAL_CHARACTER, token);
            continue;
        }

        /* Unknown character */
        token[0] = ch;
        token[1] = '\0';

        print_token(TOKEN_UNKNOWN, token);
    }
}