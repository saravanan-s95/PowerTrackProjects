#ifndef LEXER_H
#define LEXER_H

#define MAX_KEYWORDS 32
#define MAX_TOKEN_SIZE 100

#define FAILURE 0
#define SUCCESS 1

typedef enum {
    PREPROCESSOR_DIRECTIVE,
    KEYWORD,
    OPERATOR,
    SPECIAL_CHARACTER,
    CONSTANT,
    IDENTIFIER,
    CHAR_LITERAL,
    STRING_LITERAL,
    CHAR_LEXICAL_ERROR,
    STRING_LEXICAL_ERROR,
    END_OF_FILE,
    UNKNOWN
} TokenType;

typedef struct {
    char lexeme[MAX_TOKEN_SIZE];
    TokenType type;
} Token;

//To initialize the lexer
int initializeLexer(const char* filename);

//To get the token
Token getNextToken();

//To categorize the token type
void categorizeToken(Token* token);

//To check whether given string is keyword
int isKeyword(const char* str);

//To check whether given character is operator
int isOperator(const char ch);

//To check whether given character is special character
int isSpecialCharacter(const char ch);

//To check whether given string is constant
int isConstant(const char* str);

//To check whether given string is identifier
int isIdentifier(const char* str);

//To convert based on enum to string to display in terminal
const char* typeToString(TokenType type);

#endif
