#ifndef LEXER_H
#define LEXER_H

#define MAX_KEYWORDS 20
#define MAX_TOKEN_SIZE 100

typedef enum {
    KEYWORD,
    IDENTIFIER,
    OPERATOR,
    SPECIAL_CHARACTER,
    NUM_CONSTANT,
    CHAR_CONSTANT,
    HEXADECIMAL,
    BINARY,
    OCTAL,
    STRING,
    UNKNOWN
} TokenType;

typedef struct {
    char lexeme[MAX_TOKEN_SIZE];
    TokenType type;
} Token;

void initializeLexer(const char* filename);
Token getNextToken();
void categorizeToken(Token* token);
int isKeyword(const char* str);
int isOperator(const char* str);
int isSpecialCharacter(char ch);
int isCharConstant(const char* str);
int isNumConstant(const char* str);
int isHexadecimal(const char* str);
int isOctal(const char* str);
int isBinary(const char* str);
int isString(const char* str);
int isIdentifier(const char* str);

#endif
