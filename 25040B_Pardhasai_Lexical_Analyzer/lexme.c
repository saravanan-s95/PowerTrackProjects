#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"main.h"
void get_next_lexme(FILE *fp, char lexme[])
{
    char ch;          // Stores the current character
    short i = 0;      // Index for lexeme array

    skip_white_spaces(fp);      // Skip leading white spaces

    ch = fgetc(fp);      // Read first character

    if (ch == '\n')
        line_no++;      // Increment line number

    if (ch == EOF)
    {
        lexme[0] = '\0';      // Mark end of file
        return;
    }

    // Check for preprocessor directives
    if (ch == '#')
    {
        lexme[i++] = ch;      // Store '#'

        ch = fgetc(fp);

        while (isalpha(ch) || ch == '_')      // Read directive name
        {
            lexme[i++] = ch;
            ch = fgetc(fp);
        }

        lexme[i] = '\0';      // Terminate lexeme

        if (ch != EOF)
            ungetc(ch, fp);      // Push back extra character

        return;
    }

    // Check for header file
    if (ch == '<')
    {
        lexme[i++] = ch;      // Store '<'

        ch = fgetc(fp);

        while (ch != EOF)      // Read until '>'
        {
            lexme[i++] = ch;

            if (ch == '>')
                break;

            ch = fgetc(fp);
        }

        lexme[i] = '\0';      // Terminate lexeme
        return;
    }

    // Check for string literal
    if (ch == '"')
    {
        lexme[i++] = ch;      // Store opening quote

        ch = fgetc(fp);

        while (ch != EOF)      // Read until closing quote
        {
            lexme[i++] = ch;

            if (ch == '"')
                break;

            ch = fgetc(fp);
        }

        lexme[i] = '\0';      // Terminate string
        return;
    }

    // Check for operators
    if (strchr("+-*/=%<>!", ch))
    {
        lexme[0] = ch;      // Store operator
        lexme[1] = '\0';
        return;
    }

    // Check for special symbols
    if (strchr(";:,(){}[]", ch))
    {
        lexme[0] = ch;      // Store special symbol
        lexme[1] = '\0';
        return;
    }

    // Check for identifiers and keywords
    if (isalpha(ch) || ch == '_')
    {
        while (isalnum(ch) || ch == '_')      // Read identifier
        {
            lexme[i++] = ch;
            ch = fgetc(fp);
        }

        ungetc(ch, fp);      // Push back delimiter
    }

    // Check for numeric constants
    else if (isdigit(ch))
    {
        int dot_count = 0;      // Count decimal points

        while (isalnum(ch) || ch == '.' || ch == '_')      // Read number
        {
            if (ch == '.')
                dot_count++;

            lexme[i++] = ch;
            ch = fgetc(fp);
        }

        if (ch != EOF)
            ungetc(ch, fp);      // Push back delimiter
    }

    // Collect invalid or unknown lexemes
    else
    {
        lexme[i++] = ch;      // Store first invalid character

        ch = fgetc(fp);

        while (ch != EOF &&
               ch != ' ' &&
               ch != '\t' &&
               ch != '\n' &&
               !strchr("+-*/=%<>!;:,(){}[]", ch))
        {
            lexme[i++] = ch;
            ch = fgetc(fp);
        }

        if (ch != EOF)
            ungetc(ch, fp);      // Push back delimiter
    }

    lexme[i] = '\0';      // Terminate lexeme
}

void skip_white_spaces(FILE *ws)
{
    int ch;      // Stores each character read from the file

    while ((ch = fgetc(ws)) != EOF)      // Read characters until EOF
    {
        if (ch == '\n')
            line_no++;      // Increment line number for every new line

        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r')      // Check for first non-whitespace character
        {
            ungetc(ch, ws);      // Push the character back into the stream
            break;      // Stop skipping whitespaces
        }
    }
}

void print_type(int type, char lexme[])
{
    printf(BLUE "%-5d " RESET
           YELLOW "%-20s " RESET,
           line_no, lexme);      // Print line number and lexeme

    switch (type)      // Print the corresponding token type
    {
        case PREPROCESSOR_DIRECTIVE:
            printf(MAGENTA "PREPROCESSOR_DIRECTIVE\n" RESET);      // Preprocessor directive
            break;

        case HEADER_FILE:
            printf(CYAN "HEADER_FILE\n" RESET);      // Header file
            break;

        case KEYWORD:
            printf(GREEN "KEYWORD\n" RESET);      // C keyword
            break;

        case IDENTIFIER:
            printf(WHITE "IDENTIFIER\n" RESET);      // Identifier
            break;

        case NUMBER:
            printf(BRIGHT_BLUE "NUMBER\n" RESET);      // Numeric constant
            break;

        case OPERATOR:
            printf(MAGENTA "OPERATOR\n" RESET);      // Operator
            break;

        case SPECIAL_SYMBOL:
            printf(BRIGHT_CYAN "SPECIAL SYMBOL\n" RESET);      // Special symbol
            break;

        case STRING_LITERAL:
            printf(BRIGHT_GREEN "STRING_LITERAL\n" RESET);      // String literal
            break;

        default:
            printf(BRIGHT_RED "INVALID TOKEN\n" RESET);      // Invalid or unrecognized token
            break;
    }
}