#include <stdio.h>
#include "lexer.h"

extern FILE *fp;

// Returns the next token from the input file
Token getNextToken()
{
    Token token;
    char braces[100];
    static int top = -1;
    int i = 0;
    char ch;

    // Skip whitespace characters
    while ((ch = fgetc(fp)) != EOF && (isspace(ch)));

    // Skip preprocessor directives
    if (ch == '#')
    {
        while ((ch = fgetc(fp)) != '\n');
        token.type = SKIP;
        return token;
    }
    // Store opening braces
    if (ch == '(' || ch == '{' || ch == '[')
    {
        braces[++top] = ch;
    }
    // Check matching closing braces
    else if (ch == ')' || ch == ']' || ch == '}')
    {
        char temp = braces[top];
        if ((ch == ')' && temp != '(') || (ch == '}' && temp != '{') || (ch == ']' && temp != '['))
        {
            printf(RED"\nError: Braces opened but not closed.\n\n"RESET);
            token.type = ERROR;
            token.lexeme[i] = '\0';
            return token;
        }
        top--;
    }
    // Check for end of file
    if (ch == EOF)
    {
        strcpy(token.lexeme, "EOF");
        token.type = UNKNOWN;
        return token;
    }
    // Process string literals
    if (ch == '"')
    {
        token.lexeme[i++] = ch;

        while ((ch = fgetc(fp)) != EOF && ch != '"')
        {
            token.lexeme[i++] = ch;
        }

        if (ch == EOF)
        {
            printf(RED"\nError: Double quotes opened but not closed.\n\n"RESET);
            token.type = ERROR;
            token.lexeme[i] = '\0';
            return token;
        }

        token.lexeme[i++] = '"';
        token.lexeme[i] = '\0';
        token.type = STRING_LITERAL;
        return token;
    }

    // Process character constants
    if (ch == '\'')
    {
        token.lexeme[i++] = ch;
        ch = fgetc(fp);
        token.lexeme[i++] = ch;

        if ((ch = fgetc(fp)) != '\'')
        {
            printf(RED"\nError: Single quotes opened but not closed.\n\n"RESET);
            token.type = ERROR;
            token.lexeme[i] = '\0';
            return token;
        }

        token.lexeme[i++] = '\'';
        token.lexeme[i] = '\0';
        token.type = CHARACTER_CONSTANT;
        return token;
    }

    // Process identifiers
    if (isalpha(ch) || ch == '_')
    {
        token.lexeme[i++] = ch;

        while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_' || ch == '.'))
        {
            token.lexeme[i++] = ch;
        }

        token.lexeme[i] = '\0';

        if (ch != EOF)
            ungetc(ch, fp);

        if ((isIdentifier(token.lexeme)) == 0)
        {
            printf(RED"\nError: Invalid Identifier\n\n"RESET);
            token.type = ERROR;
            return token;
        }
    }
    // Process numeric constants
    else if (isdigit(ch))
    {
        token.lexeme[i++] = ch;

        while ((ch = fgetc(fp)) != EOF && (isdigit(ch) || ch == '.' || ch == 'x'))
        {
            token.lexeme[i++] = ch;
        }

        if (isalpha(ch))
        {
            token.lexeme[i++] = ch;

            while ((ch = fgetc(fp)) != EOF && (isalnum(ch) || ch == '_'))
            {
                token.lexeme[i++] = ch;
            }
        }

        token.lexeme[i] = '\0';

        if ((isNumericConst(token.lexeme)) == 0)
        {
            printf(RED"Error: Invalid constant %s\n"RESET, token.lexeme);
            token.type = ERROR;
            return token;
        }

        if (ch != EOF)
            ungetc(ch, fp);
    }
    // Process operators
    else if (isOperator(ch))
    {
        token.lexeme[i++] = ch;

        char next = fgetc(fp);

        if ((ch == '=' && next == '=') || (ch == '!' && next == '=') ||
            (ch == '<' && next == '=') || (ch == '>' && next == '=') ||
            (ch == '+' && next == '+') || (ch == '-' && next == '-') ||
            (ch == '&' && next == '&') || (ch == '|' && next == '|'))
        {
            token.lexeme[i++] = next;
        }
        else
        {
            if (next != EOF)
                ungetc(next, fp);
        }

        token.lexeme[i] = '\0';
    }
    // Process special characters
    else if (isSpecialCharacter(ch))
    {
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
    }
    // Handle unknown characters
    else
    {
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';
    }

    // Assign token type
    categorizeToken(&token);

    return token;
}

//Prints the token and its corresponding type
void printToken(Token token)
{
    char *names[] = {"KEYWORD", "OPERATOR", "SPECIAL_CHARACTER", "CONSTANT", "IDENTIFIER", "STRING_LITERAL", "CHARACTER_CONSTANT", "UNKNOWN"};

    printf("%-15s : %s\n", token.lexeme, names[token.type]);
}
