#ifndef LEXER_H
#define LEXER_H

#define MAX_KEYWORDS 20
#define MAX_TOKEN_SIZE 100
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
typedef enum {
    KEYWORD,
    OPERATOR,
    SPECIAL_CHARACTER,
    CONSTANT,
    IDENTIFIER,
    STRING_LITERAL,
    CHARACTER_CONSTANT,
    UNKNOWN,
    SKIP,
    ERROR
} TokenType;

typedef struct {
    char lexeme[MAX_TOKEN_SIZE];
    TokenType type;
} Token;


// Color macros
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"


int initializeLexer(const char* filename);
Token getNextToken();
void categorizeToken(Token* token);
int isKeyword(const char* str);
int isOperator(char);
int isSpecialCharacter(char ch);
int print_token(Token * token);
int isNumericConst(char *str);
int isIdentifier(const char* str);
void printToken(Token token);
int validate(char *str,int);
//FILE *fp;
#endif
