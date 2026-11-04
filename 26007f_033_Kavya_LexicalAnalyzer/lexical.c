#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexical.h"

static const char *keywords[] =
{
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "inline", "int", "long",
    "register", "restrict", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "_Bool"
};

static const char *operators[] =
{
    ">>=", "<<=", "...", "->", "++", "--", "==", "!=",
    "<=", ">=", "&&", "||", "+=", "-=", "*=", "/=",
    "%=", "&=", "|=", "^=", "<<", ">>",
    "+", "-", "*", "/", "%", "=", "<", ">", "!",
    "&", "|", "^", "~", "?", ":"
};

static int is_keyword(const char *word)
{
    int i;

    for (i = 0; i < (int)(sizeof(keywords) / sizeof(keywords[0])); i++)
    {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }

    return 0;
}

static void advance(Lexer *lexer)
{
    if (lexer->ch == '\n')
        lexer->line++;

    lexer->ch = fgetc(lexer->fp);
}

static Token make_token(TokenType type, const char *text, int line)
{
    Token token;

    token.type = type;
    token.line = line;

    strncpy(token.lexeme, text, TOKEN_SIZE - 1);
    token.lexeme[TOKEN_SIZE - 1] = '\0';

    return token;
}

int lexer_init(Lexer *lexer, const char *filename)
{
    lexer->fp = fopen(filename, "r");

    if (lexer->fp == NULL)
        return 0;

    lexer->line = 1;
    lexer->ch = fgetc(lexer->fp);

    return 1;
}

void lexer_destroy(Lexer *lexer)
{
    if (lexer->fp != NULL)
    {
        fclose(lexer->fp);
        lexer->fp = NULL;
    }
}

static void skip_spaces_comments(Lexer *lexer)
{
    int next;

    while (lexer->ch != EOF)
    {
        if (isspace((unsigned char)lexer->ch))
        {
            advance(lexer);
        }
        else if (lexer->ch == '/')
        {
            next = fgetc(lexer->fp);

            if (next == '/')
            {
                advance(lexer);
                advance(lexer);

                while (lexer->ch != EOF && lexer->ch != '\n')
                    advance(lexer);
            }
            else if (next == '*')
            {
                advance(lexer);
                advance(lexer);

                while (lexer->ch != EOF)
                {
                    if (lexer->ch == '*')
                    {
                        advance(lexer);

                        if (lexer->ch == '/')
                        {
                            advance(lexer);
                            break;
                        }
                    }
                    else
                    {
                        advance(lexer);
                    }
                }
            }
            else
            {
                if (next != EOF)
                    ungetc(next, lexer->fp);

                break;
            }
        }
        else
        {
            break;
        }
    }
}

static int is_special_symbol(int ch)
{
    return ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == ';' || ch == ',' || ch == '.';
}

static Token read_identifier(Lexer *lexer, int line)
{
    char buffer[TOKEN_SIZE];
    int i = 0;

    while (lexer->ch != EOF &&
           (isalnum((unsigned char)lexer->ch) || lexer->ch == '_'))
    {
        if (i < TOKEN_SIZE - 1)
            buffer[i++] = (char)lexer->ch;

        advance(lexer);
    }

    buffer[i] = '\0';

    if (is_keyword(buffer))
        return make_token(KEYWORD, buffer, line);

    return make_token(IDENTIFIER, buffer, line);
}

static Token read_number(Lexer *lexer, int line)
{
    char buffer[TOKEN_SIZE];
    int i = 0;
    int is_float = 0;

    while (lexer->ch != EOF && (isalnum((unsigned char)lexer->ch) || lexer->ch == '.' || lexer->ch == '_'))
    {
        if (lexer->ch == '.' || lexer->ch == 'e' || lexer->ch == 'E' || lexer->ch == 'f' || lexer->ch == 'F')
        {
            is_float = 1;
        }

        if (i < TOKEN_SIZE - 1)
            buffer[i++] = (char)lexer->ch;

        advance(lexer);
    }

    buffer[i] = '\0';

    if (is_float)
        return make_token(FLOAT, buffer, line);

    return make_token(INTEGER, buffer, line);
}

static Token read_string(Lexer *lexer, int line, int quote)
{
    char buffer[TOKEN_SIZE];
    int i = 0;

    buffer[i++] = (char)lexer->ch;
    advance(lexer);

    while (lexer->ch != EOF && i < TOKEN_SIZE - 2)
    {
        int current = lexer->ch;

        buffer[i++] = (char)current;
        advance(lexer);

        if (current == '\\' && lexer->ch != EOF)
        {
            buffer[i++] = (char)lexer->ch;
            advance(lexer);
        }
        else if (current == quote)
        {
            break;
        }
    }

    buffer[i] = '\0';

    if (quote == '"')
        return make_token(STRING, buffer, line);

    return make_token(CHARACTER, buffer, line);
}

static Token read_operator(Lexer *lexer, int line)
{
    char buffer[4] = "";
    char candidate[4];
    int best_length = 0;
    int i;
    int j;

    buffer[0] = (char)lexer->ch;
    buffer[1] = '\0';

    for (i = 0; i < (int)(sizeof(operators) / sizeof(operators[0])); i++)
    {
        int length = (int)strlen(operators[i]);

        if (length <= best_length || operators[i][0] != lexer->ch)
            continue;

        for (j = 0; j < length; j++)
        {
            if (j == 0)
                candidate[j] = (char)lexer->ch;
            else
                candidate[j] = (char)fgetc(lexer->fp);
        }

        candidate[length] = '\0';

        if (strcmp(candidate, operators[i]) == 0)
        {
            strcpy(buffer, candidate);
            best_length = length;
        }

        for (j = length - 1; j >= 1; j--)
            ungetc(candidate[j], lexer->fp);
    }

    advance(lexer);

    for (i = 1; i < best_length; i++)
        advance(lexer);

    return make_token(OPERATOR, buffer, line);
}

Token get_next_token(Lexer *lexer)
{
    int line;
    char buffer[2];

    skip_spaces_comments(lexer);

    line = lexer->line;

    if (lexer->ch == EOF)
        return make_token(END_OF_FILE, "EOF", line);

    if (lexer->ch == '#')
    {
        char text[TOKEN_SIZE];
        int i = 0;

        while (lexer->ch != EOF && lexer->ch != '\n')
        {
            if (i < TOKEN_SIZE - 1)
                text[i++] = (char)lexer->ch;

            advance(lexer);
        }

        text[i] = '\0';

        return make_token(PREPROCESSOR, text, line);
    }

    if (isalpha((unsigned char)lexer->ch) || lexer->ch == '_')
        return read_identifier(lexer, line);

    if (isdigit((unsigned char)lexer->ch))
        return read_number(lexer, line);

    if (lexer->ch == '"')
        return read_string(lexer, line, '"');

    if (lexer->ch == '\'')
        return read_string(lexer, line, '\'');

    if (is_special_symbol(lexer->ch))
    {
        buffer[0] = (char)lexer->ch;
        buffer[1] = '\0';

        advance(lexer);

        return make_token(SPECIAL_SYMBOL, buffer, line);
    }

    if (strchr("+-*/%=<>!&|^~?:", lexer->ch) != NULL)
        return read_operator(lexer, line);

    buffer[0] = (char)lexer->ch;
    buffer[1] = '\0';

    advance(lexer);

    return make_token(UNKNOWN, buffer, line);
}

const char *get_token_name(TokenType type)
{
    switch (type)
    {
        case KEYWORD:
            return "KEYWORD";
        case IDENTIFIER:
            return "IDENTIFIER";
        case INTEGER:
            return "INTEGER";
        case FLOAT:
            return "FLOAT";
        case STRING:
            return "STRING";
        case CHARACTER:
            return "CHARACTER";
        case OPERATOR:
            return "OPERATOR";
        case SPECIAL_SYMBOL:
            return "SPECIAL_SYMBOL";
        case PREPROCESSOR:
            return "PREPROCESSOR";
        case UNKNOWN:
            return "UNKNOWN";
        case END_OF_FILE:
            return "EOF";
        default:
            return "UNKNOWN";
    }
}
