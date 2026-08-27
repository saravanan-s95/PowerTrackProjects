#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

//List of C keywords
static const char* keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"
};

//List of valid operators
static const char* operators = "+-*/%=!<>|&";
//List of special characters
static const char* specialCharacters = ",;{}()[]";

//Checks whether the string is a keyword
int isKeyword(const char* str)
{
    for(int i=0;i<20;i++)
    {
        if((strcmp(keywords[i],str))==0)
           return 1;
        //i++;
    }
    return 0;
}

//Checks for special characters
int isSpecialCharacter(char ch)
{
    int i=0;
    while(specialCharacters[i])
    {
        if(specialCharacters[i]==ch)
        {
            return 1;
        }
        i++;
    }
    return 0;
}

//Validates numeric constants
int isNumericConst(char *str)
{
    int i = 0;
    int j=0;
    if(str[0]=='+' || str[0]=='-') //checking the constant is positive or negative
    {
        i++;
    }
    else if(str[0]=='0' && str[1]=='x') //this for hexadecimal constant
    {
        int i=2;
        while(str[i])
        {
            if((isdigit(str[i])) || ((str[i]>='A' && str[i]<='F')||(str[i]>='a' && str[i]<='f')))
            {
                i++;
            }
            else 
               return 0;
        }
        return 1;
    }
    if(str[0]=='0' && isdigit(str[1])) //this is for octal
    {
        int j=0;
        while (isdigit(str[j]) || str[j]=='.')
        {
            if(str[j]=='.')
            {
                break;
            }
           if(str[j]>='0'&&str[j]<='8')
           {
             j++;
           }
           else
              return 0;
        }
        return 1;
    }
    j=0;
    while(str[i]) //float values
    {
    
        if(str[i]=='.')
        {
            j++;
            if(j>1)
            {
                return 0;
            }
            if(j==1)
            {
                i++;
                continue;
            }
        }
        if(j==1)
        {
            char *ptr;
            strtof(str,&ptr);
           // printf("ptr:- %s\n",ptr);
            if(strlen(ptr)>2)
            {
                return 0;
            }
            if(*ptr=='\0' || *ptr=='f'||*ptr=='F')
            {
                return 1;
            }
            else
            {
               // printf("shubham\n");
                return 0;
            }
        }    
        i++;
    }
    i=0;
    int count=0;
    while(str[i]) //decimal 
    {
        if(!isdigit(str[i]))
        {
            return 0;
        }
        else{
            count++;
        }
        i++;
    }
    if(strlen(str)==count)
    {
        return 1;
    }
    return 0;

}

//Checks whether the character is an operator
int isOperator(char ch)
{
    int i=0;
    while(operators[i])
    {
        if(operators[i]==ch)
        {
            return 1;
        }
        i++;
    }
    return 0;
}

//Validates identifiers
int isIdentifier(const char *str)
{
    if(!(isalpha(str[0]) || str[0]=='_'))
        return 0;

    for(int i=1; str[i]; i++)
    {
        if(!(isalnum(str[i]) || str[i]=='_'))
        {
            printf(RED"\nError: Invalid identifier\n\n"RESET);
            return 0;
        }
    }

    return 1;
}

//Assigns the appropriate token type
void categorizeToken(Token *token)
{
    if(isKeyword(token->lexeme))
    {
        token->type = KEYWORD;
    }
    else if(isNumericConst(token->lexeme))
    {
        token->type = CONSTANT;
    }
    else if(strlen(token->lexeme)==1 && isOperator(token->lexeme[0]))
    {
        token->type = OPERATOR;
    }
    else if(strlen(token->lexeme)==1 && isSpecialCharacter(token->lexeme[0]))
    {
        token->type = SPECIAL_CHARACTER;
    }
    else if(isIdentifier(token->lexeme))
    {
        token->type = IDENTIFIER;
    }
    else
    {
        token->type = UNKNOWN;
        //printf("error is detected\n");

        
    }
}