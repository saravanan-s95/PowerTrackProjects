#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static const char* keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"
};

static const char* operators = "+-*/%=!<>|&";
static const char* specialCharacters = ",;{}()[]";

FILE *ptr = NULL;

void initializeLexer(const char* filename){
    ptr = fopen(filename , "r");        //open source file in read mode
}

Token getNextToken()
{
    int i=0;
    int ch;
    Token token;
    
    ch = getc(ptr);                     //read first character

    while(ch == ' ' || ch == '\n' || ch == '\t'){   //skip whitespaces
        ch = getc(ptr);
    }

    while(ch == '#'){                   //skip preprocessor directives
        while((ch = getc(ptr)) != '\n' && ch != EOF);   //discard current line

        ch = getc(ptr);                 //read next useful character

        while(ch == ' ' || ch == '\n' || ch == '\t'){   //skip remaining whitespaces
            ch = getc(ptr);
        }
    }

    token.lexeme[i++] = ch;

    if(isalpha(ch)){                    //identifier or keyword extraction
        while(isalpha(ch = getc(ptr)) || isdigit(ch) || ch == '_')
          token.lexeme[i++] = ch;  
        fseek(ptr,-1,SEEK_CUR);
    }
    else if(isdigit(ch)){               //numeric constant extraction
        while(isdigit(ch = getc(ptr)) || ch == '.' || isalpha(ch))
            token.lexeme[i++] = ch;
        fseek(ptr,-1,SEEK_CUR);
    }
    else if(ch == '\''){                //character constant extraction
        while((ch = getc(ptr)) != '\'')
            token.lexeme[i++] = ch; 
        token.lexeme[i++] = ch;
    }
    else if(ch == '"'){                 //string literal extraction
        while((ch = getc(ptr)) != '"')
            token.lexeme[i++] = ch; 
        token.lexeme[i++] = ch;
    }
    else if(ispunct(ch)){
        if(strchr(operators,ch) != NULL){   //operator extraction
            if(strchr(operators,ch = getc(ptr)) != NULL){
                token.lexeme[i++] = ch; 
            }    
        }
    }

    token.lexeme[i] = '\0';             //terminate extracted token

    categorizeToken(&token);            //identify token category

    return token;
}

void categorizeToken(Token* token)
{
    if(isKeyword(token->lexeme))        //check keyword
        token->type = KEYWORD;
    else if(isIdentifier(token->lexeme)) //check identifier
        token->type = IDENTIFIER;
    else if(isOperator(token->lexeme))  //check operator
        token->type = OPERATOR;
    else if(isSpecialCharacter(token->lexeme[0]))   //check special character
        token->type = SPECIAL_CHARACTER;
    else if(isCharConstant(token->lexeme))   //check character constant
        token->type = CHAR_CONSTANT;
    else if(isString(token->lexeme))    //check string literal
        token->type = STRING;
    else if(isHexadecimal(token->lexeme))    //check hexadecimal number
        token->type = HEXADECIMAL;
    else if(isBinary(token->lexeme))    //check binary number
        token->type = BINARY;
    else if(isOctal(token->lexeme))     //check octal number
        token->type = OCTAL;
    else if(isNumConstant(token->lexeme))    //check decimal number
        token->type = NUM_CONSTANT;
    else
       token->type = UNKNOWN;           //invalid token
}

int isKeyword(const char* str){         //validate C keyword
    for(int i=0; i<MAX_KEYWORDS ;i++){
        if(strcmp(keywords[i],str) == 0)
            return 1;
    }
    return 0;
}

int isIdentifier(const char* str){          //validate identifier naming rules
    if(isalpha(str[0]) || str[0] == '_'){
        for(int i=1 ; str[i] ;i++){
            if(!isalpha(str[i]) && !isdigit(str[i]) && str[i] != '_')
                return 0;
        }
        return 1;
    }
    else
        return 0;
}

int isOperator(const char* str){    //validate operators
    char *arr[31] = {".","->","++","--","&", "*", "+", "-", "~", "!", "/", "%", "<<", ">>","<",">","<=", ">=", "==", "!=", "^","|", "&&", "||", "=", "*=","/=","%=", "+=", "-=", ","};
    for(int i=0;i<31;i++){
        if(strcmp(str,arr[i])==0)
            return 1;
    }
    return 0;
}

int isSpecialCharacter(char ch){         //validate special symbols
    if(strchr(specialCharacters,ch) != NULL)
        return 1;
    else
        return 0;
}

int isCharConstant(const char* str){        //validate character constants
    int len = strlen(str);
    if(str[0] != '\'' || str[len-1] != '\'')
        return 0;
    if(len > 3){
        if(str[1] != '\\')
            return 0;
    }
    return 1;
}

int isNumConstant(const char* str){          //validate decimal constants
    if(str[0] == '0')
        return 0;
    for(int i=0; str[i] ;i++){
        if(!isdigit(str[i]) && str[i] != '.')
            return 0;
    }
    return 1;
}

int isHexadecimal(const char* str){         //validate hexadecimal constants
    if(str[0] != '0' || (str[1] != 'X' && str[1] != 'x'))   //should start  with 0x.. or 0X..
        return 0;
    int len = strlen(str);
    if(len < 3)
        return 0;
    for(int i=2 ; str[i] ; i++){
        if(!isxdigit(str[i]))
            return 0;
    }
    return 1;
}

int isOctal(const char* str){       //validate octal constants
    if(str[0] != '0')   //must start with 0
        return 0;
    int len = strlen(str);
    if(len < 2)
        return 0;
    for(int i=0; str[i] ;i++){
        if(str[i] < '0' || str[i] > '7')
            return 0;
    }
    return 1;
}

int isBinary(const char* str){          //validate binary constants
    if(str[0] != '0' || (str[1] != 'b'))   //should start  with 0b..
        return 0;
    int len = strlen(str);
    if(len < 3)
        return 0;
    for(int i=2; str[i] ;i++){
        if(str[i] < '0' || str[i] > '1')
            return 0;
    }
    return 1;
}

int isString(const char* str){      //validate string literals
    int len = strlen(str);
    if(str[0] != '"' || str[len-1] != '"')
        return 0;
    return 1;
}