#ifndef ANALYZER_H
#define ANALYZER_H

#include<stdio.h>
#include<stdbool.h>

typedef enum
{
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_FLOAT,
    TOKEN_OPERATOR,
    TOKEN_SPECIAL_SYMBOL,
    TOKEN_STRING,
    TOKEN_CHARACTER,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct
{
    TokenType type;
    char lexeme[256];
    int line;
} Token;

typedef struct
{
    FILE *source;
    int line;
    int currentChar;
} Lexer;


// lexer.c
void initLexer(Lexer *lexer, FILE *source);
Token getNextToken(Lexer *lexer);
void skipWhitespace(Lexer *lexer);
void skipComment(Lexer *lexer);

// token.c
bool isKeyword(const char *word);
bool isOperator(char ch);
bool isSpecialSymbol(char ch);
TokenType classifyToken(const char *lexeme);
void printToken(Token token);

// error.c
void reportError(int line, const char *message);

// for checking file formate
int check_file_formate(const char *filename);


#endif