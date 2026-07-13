#include <stdio.h>      // Standard input/output functions
#include "main.h"       // User-defined declarations
#include <string.h>     // String handling functions
#include <ctype.h>      // Character checking functions

int classify_token(char *lexeme)
{
    int i;      // Loop counter

    // Check for preprocessor directives
    if ((strcmp(lexeme, "#include") == 0) || (strcmp(lexeme, "#define") == 0) || (strcmp(lexeme, "#ifndef") == 0) || (strcmp(lexeme, "#endif") == 0))
        return PREPROCESSOR_DIRECTIVE;

    // Check for header file names
    if (lexeme[0] == '<' && lexeme[strlen(lexeme) - 1] == '>' && strstr(lexeme, ".h") != NULL)
        return HEADER_FILE;

    // Check whether the lexeme is a keyword
    for (i = 0; i < NUM_KEYWORDS; i++)
    {
        if (strcmp(lexeme, keywords[i]) == 0)
            return KEYWORD;
    }

    // Check whether the lexeme is a valid identifier
    if (isalpha(lexeme[0]) || lexeme[0] == '_')
    {
        for (i = 1; lexeme[i]; i++)
        {
            if (!isalnum(lexeme[i]) && lexeme[i] != '_')
                break;
        }

        if (lexeme[i] == '\0')
            return IDENTIFIER;
    }

    // Check whether the lexeme is a valid number
    if ((isdigit(lexeme[0]) || lexeme[0] == '.') && check_error(lexeme) == VALID_LEXEME)
        return NUMBER;

    // Check for a valid string literal
    if (lexeme[0] == '"' && lexeme[strlen(lexeme) - 1] == '"')
        return STRING_LITERAL;

    // Check for an unterminated string literal
    if (lexeme[0] == '"')
    {
        int len = strlen(lexeme);      // Store string length

        if (lexeme[len - 1] != '"')
            return UNTERMINATED_STRING;
    }

    // Check whether the lexeme is an operator
    for (i = 0; i < num_relational_operators; i++)
    {
        if (strcmp(lexeme, relational_operators[i]) == 0)
            return OPERATOR;
    }

    // Check whether the lexeme is a special symbol
    if (strlen(lexeme) == 1 && strchr(special, lexeme[0]))
        return SPECIAL_SYMBOL;

    // Return invalid token if no category matches
    return INVALID_TOKEN;
}