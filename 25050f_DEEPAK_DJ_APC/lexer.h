#ifndef LEXER_H
#define LEXER_H

#define MAX_KEYWORDS 20
#define MAX_TOKEN_SIZE 100

#include<stdio.h>
#include<string.h>
#include<ctype.h>

typedef enum {
    KEYWORD,
    OPERATOR,
    SPECIAL_CHARACTER,
    CONSTANT,
    IDENTIFIER,
    PREPROCESSOR_DIRECTIVE,
    COMMENT,
    STRING_LITERAL,
    UNKNOWN
} TokenType;

typedef struct {
    char lexeme[MAX_TOKEN_SIZE];
    TokenType type;
} Token;

void initializeLexer(FILE *fp);
void categorizeToken(Token* token);
int isKeyword(const char* str);
int isOperator(const char* str);
int isSpecialCharacter(char ch);
int isConstant(const char* str);
int isIdentifier(const char* str);
int isPreprocessorDirective(const char* str);
int isComment(const char* str);
char *getTokenString(int num);
int isStringLiteral(const char* str);
char *specialchars(char ch);

#endif
