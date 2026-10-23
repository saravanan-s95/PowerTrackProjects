
#include "analyzer.h"
#include <string.h>
#include"ctype.h"

int check_file_formate(const char *filename)
{
    const char *extension = strrchr(filename, '.');

    if (extension != NULL && strcmp(extension, ".c") == 0)
    {
        return 1;
    }

    return 0;
}

void initLexer(Lexer *lexer, FILE *source)
{
    lexer->source = source;
    lexer->line = 1;
    lexer->currentChar = fgetc(source);
}

void skipWhitespace(Lexer *lexer)
{
    while (lexer->currentChar != EOF &&
           isspace((unsigned char)lexer->currentChar))
    {
        if (lexer->currentChar == '\n')
        {
            lexer->line++;
        }

        lexer->currentChar = fgetc(lexer->source);
    }
}

void skipComment(Lexer *lexer)
{
    int nextChar;

    if (lexer->currentChar != '/')
        return;

    nextChar = fgetc(lexer->source);

    // Single-line comment: //
    if (nextChar == '/')
    {
        while (lexer->currentChar != EOF &&
               lexer->currentChar != '\n')
        {
            lexer->currentChar = fgetc(lexer->source);
        }
    }

    // Multi-line comment: /* ... */
    else if (nextChar == '*')
    {
        lexer->currentChar = fgetc(lexer->source);

        while (lexer->currentChar != EOF)
        {
            if (lexer->currentChar == '\n')
                lexer->line++;

            if (lexer->currentChar == '*')
            {
                lexer->currentChar = fgetc(lexer->source);

                if (lexer->currentChar == '/')
                {
                    lexer->currentChar = fgetc(lexer->source);
                    return;
                }

                continue;
            }

            lexer->currentChar = fgetc(lexer->source);
        }

        reportError(lexer->line, "Unterminated multi-line comment");
    }

    // Not a comment: restore lookahead and retain '/'
    else
    {
        if (nextChar != EOF)
            ungetc(nextChar, lexer->source);
    }
}



Token getNextToken(Lexer *lexer)
{
    Token token;
    int i = 0;

    // 1. Skip whitespace and comments
    while (1)
    {
        skipWhitespace(lexer);

        if (lexer->currentChar == '/')
        {
            int nextChar = fgetc(lexer->source);

            if (nextChar != EOF)
                ungetc(nextChar, lexer->source);

            if (nextChar == '/' || nextChar == '*')
            {
                skipComment(lexer);
                continue;
            }
        }

        break;
    }

    // 2. Initialize token
    token.lexeme[0] = '\0';
    token.line = lexer->line;
    token.type = TOKEN_ERROR;

    // 3. Handle EOF
    if (lexer->currentChar == EOF)
    {
        token.type = TOKEN_EOF;
        return token;
    }

    // 4. Handle identifiers and keywords
    if (isalpha((unsigned char)lexer->currentChar) ||
        lexer->currentChar == '_')
    {
        int overflow = 0;
        i = 0;

        while (lexer->currentChar != EOF &&
               (isalnum((unsigned char)lexer->currentChar) ||
                lexer->currentChar == '_'))
        {
            if (i < (int)sizeof(token.lexeme) - 1)
                token.lexeme[i++] = (char)lexer->currentChar;
            else
                overflow = 1;

            lexer->currentChar = fgetc(lexer->source);
        }

        token.lexeme[i] = '\0';

        if (overflow)
        {
            reportError(token.line, "Identifier is too long");
            return token;
        }

        token.type = isKeyword(token.lexeme)
                     ? TOKEN_KEYWORD
                     : TOKEN_IDENTIFIER;

        return token;
    }

    // 5. Handle integer constants
    if (isdigit((unsigned char)lexer->currentChar))
    {
        int overflow = 0;
        i = 0;

        while (lexer->currentChar != EOF &&
               isdigit((unsigned char)lexer->currentChar))
        {
            if (i < (int)sizeof(token.lexeme) - 1)
                token.lexeme[i++] = (char)lexer->currentChar;
            else
                overflow = 1;

            lexer->currentChar = fgetc(lexer->source);
        }

        token.lexeme[i] = '\0';
        token.type = overflow ? TOKEN_ERROR : TOKEN_INTEGER;

        if (overflow)
            reportError(token.line, "Integer constant is too long");

        return token;
    }

    // 6. Handle string literals
    if (lexer->currentChar == '"')
    {
        int closed = 0;
        int overflow = 0;
        i = 0;

        // Store opening quote
        token.lexeme[i++] = '"';
        lexer->currentChar = fgetc(lexer->source);

        while (lexer->currentChar != EOF &&
               lexer->currentChar != '\n')
        {
            if (lexer->currentChar == '"')
            {
                if (i < (int)sizeof(token.lexeme) - 1)
                    token.lexeme[i++] = '"';
                else
                    overflow = 1;

                lexer->currentChar = fgetc(lexer->source);
                closed = 1;
                break;
            }

            if (i < (int)sizeof(token.lexeme) - 1)
                token.lexeme[i++] = (char)lexer->currentChar;
            else
                overflow = 1;

            //Handle escaped characters such as \" and //
            if (lexer->currentChar == '\\')
            {
                lexer->currentChar = fgetc(lexer->source);

                if (lexer->currentChar == EOF ||
                    lexer->currentChar == '\n')
                    break;

                if (i < (int)sizeof(token.lexeme) - 1)
                    token.lexeme[i++] = (char)lexer->currentChar;
                else
                    overflow = 1;
            }

            lexer->currentChar = fgetc(lexer->source);
        }

        token.lexeme[i] = '\0';

        if (!closed || overflow)
        {
            token.type = TOKEN_ERROR;
            reportError(token.line,
                        !closed ? "Unterminated string literal"
                                : "String literal is too long");
        }
        else
        {
            token.type = TOKEN_STRING;
        }

        return token;
    }

    // 7. Handle operators
    if (strchr("+-*/%=<>!&|^~", lexer->currentChar) != NULL)
    {
        char first = (char)lexer->currentChar;

        token.lexeme[0] = first;
        token.lexeme[1] = '\0';

        lexer->currentChar = fgetc(lexer->source);

        // Recognize two-character operators
        if ((first == '=' && lexer->currentChar == '=') ||
            (first == '!' && lexer->currentChar == '=') ||
            (first == '<' && (lexer->currentChar == '=' ||
                              lexer->currentChar == '<')) ||
            (first == '>' && (lexer->currentChar == '=' ||
                              lexer->currentChar == '>')) ||
            (first == '+' && (lexer->currentChar == '+' ||
                              lexer->currentChar == '=')) ||
            (first == '-' && (lexer->currentChar == '-' ||
                              lexer->currentChar == '=' ||
                              lexer->currentChar == '>')) ||
            (first == '*' && lexer->currentChar == '=') ||
            (first == '/' && lexer->currentChar == '=') ||
            (first == '%' && lexer->currentChar == '=') ||
            (first == '&' && (lexer->currentChar == '&' ||
                              lexer->currentChar == '=')) ||
            (first == '|' && (lexer->currentChar == '|' ||
                              lexer->currentChar == '=')) ||
            (first == '^' && lexer->currentChar == '='))
        {
            token.lexeme[1] = (char)lexer->currentChar;
            token.lexeme[2] = '\0';

            lexer->currentChar = fgetc(lexer->source);
        }
        else if ((first == '<' && token.lexeme[0] == '<') ||
                 (first == '>' && token.lexeme[0] == '>'))
        {
            /* No action needed here. */
        }

        // Handle <<= and >>=
        if ((strcmp(token.lexeme, "<<") == 0 ||
             strcmp(token.lexeme, ">>") == 0) &&
            lexer->currentChar == '=')
        {
            token.lexeme[2] = '=';
            token.lexeme[3] = '\0';
            lexer->currentChar = fgetc(lexer->source);
        }

        token.type = TOKEN_OPERATOR;
        return token;
    }

    // 8. Handle special symbols
    if (strchr("(){}[];,.:?#", lexer->currentChar) != NULL)
    {
        token.lexeme[0] = (char)lexer->currentChar;
        token.lexeme[1] = '\0';
        token.type = TOKEN_SPECIAL_SYMBOL;

        lexer->currentChar = fgetc(lexer->source);
        return token;
    }

    // 9. Handle invalid characters
    token.lexeme[0] = (char)lexer->currentChar;
    token.lexeme[1] = '\0';
    token.type = TOKEN_ERROR;

    reportError(token.line, "Invalid character");

    lexer->currentChar = fgetc(lexer->source);

    return token;
}