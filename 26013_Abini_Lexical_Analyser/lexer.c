#include "lexer.h"

#define MAX 100

/* List of C keywords */
char *keywords[] = {
    "int", "char", "float", "double", "void",
    "return", "if", "else", "for", "while",
    "do", "break", "continue", "switch", "case",
    "default", "const", "static", "long", "short",
    "signed", "unsigned", "struct", "sizeof"
};

/* Check whether the character is a letter */
int is_letter(int ch)
{
    return ((ch >= 'A' && ch <= 'Z') ||
            (ch >= 'a' && ch <= 'z'));
}

/* Check whether the character can be part of a name */
int is_name_char(int ch)
{
    return (is_letter(ch) ||
            (ch >= '0' && ch <= '9') ||
            ch == '_');
}

/* Check whether the word is a keyword */
int is_keyword(char *word)
{
    int i;

    for (i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++)
    {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }

    return 0;
}

/* Check whether the character is an operator */
int is_operator(int ch)
{
    return (ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == '%' || ch == '=' ||
            ch == '<' || ch == '>' || ch == '!' ||
            ch == '&' || ch == '|' || ch == '^' ||
            ch == '~' || ch == '?');
}

/* Check whether the character is a punctuator */
int is_punctuator(int ch)
{
    return (ch == '(' || ch == ')' ||
            ch == '{' || ch == '}' ||
            ch == '[' || ch == ']' ||
            ch == ';' || ch == ',' ||
            ch == '.' || ch == ':');
}

/* Display table header */
void print_header(void)
{
    printf("\n");
    printf("+----------------------+----------------------+\n");
    printf("| %-20s | %-20s |\n", "TOKEN TYPE", "LEXEME");
    printf("+----------------------+----------------------+\n");
}

/* Display a token */
void print_token(const char *type, const char *word)
{
    printf("| %-20s | %-20s |\n", type, word);
    printf("+----------------------+----------------------+\n");
}

/* Display a lexical error */
void print_error(const char *message, const char *word)
{
    printf("| %-20s | %-20s |\n", message, word);
    printf("+----------------------+----------------------+\n");
}

/* Lexical analyser */
void analyse(FILE *fp)
{
    int ch, next;
    int i, errors = 0;
    int closed, special_char;
    char word[MAX];

    print_header();

    ch = fgetc(fp);

    while (ch != EOF)
    {
        /* Ignore spaces, tabs and newlines */
        if (ch == ' ' || ch == '\t' ||
            ch == '\n' || ch == '\r')
        {
            ch = fgetc(fp);
            continue;
        }

        /* Identify keywords and identifiers */
        if (is_letter(ch) || ch == '_')
        {
            i = 0;

            while (ch != EOF && is_name_char(ch))
            {
                if (i < MAX - 1)
                    word[i++] = ch;

                ch = fgetc(fp);
            }

            word[i] = '\0';

            if (is_keyword(word))
                print_token("Keyword", word);
            else
                print_token("Identifier", word);

            continue;
        }

        /* Identify integer constants */
        if (ch >= '0' && ch <= '9')
        {
            i = 0;

            while (ch >= '0' && ch <= '9')
            {
                if (i < MAX - 1)
                    word[i++] = ch;

                ch = fgetc(fp);
            }

            word[i] = '\0';
            print_token("Constant", word);
            continue;
        }

        /* Identify string literals */
        if (ch == '"')
        {
            i = 0;
            closed = 0;
            special_char = 0;
            word[i++] = ch;
            ch = fgetc(fp);

            while (ch != EOF && ch != '\n')
            {
                if (i < MAX - 1)
                    word[i++] = ch;

                if (ch == '"' && !special_char)
                {
                    closed = 1;
                    ch = fgetc(fp);
                    break;
                }

                if (ch == '\\' && !special_char)
                    special_char = 1;
                else
                    special_char = 0;

                ch = fgetc(fp);
            }

            word[i] = '\0';

            if (closed)
                print_token("String Literal", word);
            else
            {
                print_error("Missing closing quote", word);
                errors++;
            }

            continue;
        }

        /* Identify character literals */
        if (ch == '\'')
        {
            i = 0;
            closed = 0;
            word[i++] = ch;
            ch = fgetc(fp);

            if (ch != EOF && ch != '\n')
            {
                if (i < MAX - 1)
                    word[i++] = ch;

                if (ch == '\\')
                {
                    ch = fgetc(fp);

                    if (ch != EOF && ch != '\n')
                    {
                        if (i < MAX - 1)
                            word[i++] = ch;

                        ch = fgetc(fp);
                    }
                }
                else
                    ch = fgetc(fp);

                if (ch == '\'')
                {
                    if (i < MAX - 1)
                        word[i++] = ch;

                    closed = 1;
                    ch = fgetc(fp);
                }
            }

            word[i] = '\0';

            if (closed)
                print_token("Character Literal", word);
            else
            {
                print_error("Invalid character literal", word);
                errors++;

                if (ch != EOF && ch != '\n')
                    ch = fgetc(fp);
            }

            continue;
        }

        /* Check for comments and division operator */
        if (ch == '/')
        {
            next = fgetc(fp);

            /* Single-line comment */
            if (next == '/')
            {
                while (ch != EOF && ch != '\n')
                    ch = fgetc(fp);

                if (ch != EOF)
                    ch = fgetc(fp);

                continue;
            }

            /* Multi-line comment */
            if (next == '*')
            {
                ch = fgetc(fp);
                closed = 0;

                while (ch != EOF)
                {
                    if (ch == '*')
                    {
                        next = fgetc(fp);

                        if (next == '/')
                        {
                            closed = 1;
                            ch = fgetc(fp);
                            break;
                        }

                        ch = next;
                    }
                    else
                        ch = fgetc(fp);
                }

                if (!closed)
                {
                    print_error("Missing comment ending", "/*");
                    errors++;
                }

                continue;
            }

            /* Identify division operator */
            print_token("Operator", "/");
            ch = next;
            continue;
        }

        /* Identify operators */
        if (is_operator(ch))
        {
            word[0] = ch;
            word[1] = '\0';

            next = fgetc(fp);

            if ((ch == '+' && (next == '+' || next == '=')) ||
                (ch == '-' && (next == '-' || next == '=')) ||
                (ch == '*' && next == '=') ||
                (ch == '%' && next == '=') ||
                (ch == '=' && next == '=') ||
                (ch == '!' && next == '=') ||
                (ch == '<' && (next == '=' || next == '<')) ||
                (ch == '>' && (next == '=' || next == '>')) ||
                (ch == '&' && (next == '&' || next == '=')) ||
                (ch == '|' && (next == '|' || next == '=')) ||
                (ch == '^' && next == '='))
            {
                word[1] = next;
                word[2] = '\0';
                ch = fgetc(fp);
            }
            else
                ch = next;

            print_token("Operator", word);
            continue;
        }

        /* Identify punctuators */
        if (is_punctuator(ch))
        {
            word[0] = ch;
            word[1] = '\0';

            print_token("Punctuator", word);
            ch = fgetc(fp);
            continue;
        }

        /* Identify unknown characters */
        word[0] = ch;
        word[1] = '\0';

        print_error("Unknown character", word);
        errors++;
        ch = fgetc(fp);
    }

    printf("----------------------------------------\n");
    printf("Total lexical errors: %d\n", errors);
}
