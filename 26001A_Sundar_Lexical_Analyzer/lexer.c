#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

//Declare an array of all keywords in C language
static const char* keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct",
    "auto", "enum", "extern", "goto", "long", "short", "register", "signed", "typedef", "union",
    "unsigned", "volatile"
};

//Declare operators and special characters in code segment
static const char* operators = "+-*/%=!<>|&";
static const char* specialCharacters = ",.?:;{}()[]";

//Declare file pointer,buffer and position in buffer as static
static FILE* fp;
static char buffer[MAX_TOKEN_SIZE];
static int pos = 0;

//Line count variable
int line = 0;

int initializeLexer(const char* filename)
{
    //Check if given file is .c extension
    char* c = strchr(filename,'.');
    if(c == NULL || strcmp(c,".c") != 0)
    {
        //Wrong file
        printf("Please pass only .c files\n");
        return FAILURE;
    }

    //Open the .c file in read mode
    fp = fopen(filename,"r");

    //If file doesnt exist print error
    if(fp == NULL)
    {
        printf("File does not exist\n");
        return FAILURE;
    }

    //If file contents empty print error
    if(fgetc(fp) == EOF)
    {
        printf("File is empty\n");
        return FAILURE;
    }
    //Rewind the file pointer to start of the position
    rewind(fp);
    printf("Lexer initialized successfully\n");
    return SUCCESS;
}

Token getNextToken()
{
    //Create the structure
    Token generate_token;
    int i = 0;
    
    while(1)
    {
        //Check for multi line comments
        static int multi_line = 0;

        //Check whether the line contents have been tokenized or not
        if(buffer[pos] == '\0')
        {
            //Store the entire line into the buffer using fgets() function
            if(fgets(buffer,sizeof(buffer),fp) == NULL)
            {
                //If multi line comment is unterminated
                if(multi_line)
                {
                    printf("Lexical Error [Line %d] : Unterminated multi line comment\n",line);
                    generate_token.type = UNKNOWN;
                    return generate_token;
                }

                //If next line doesnt exist return EOF
                generate_token.type = END_OF_FILE;
                return generate_token;
            }
            //Increment line count
            line++;
            //If it exists reset position in buffer to 0
            pos = 0;

            //Remove \n and \r 
            size_t len = strlen(buffer);
            if(len > 0 && buffer[len-1] == '\n') 
            {
                buffer[--len] = '\0';
            }
            if(len > 0 && buffer[len-1] == '\r') 
            {
                buffer[--len] = '\0';
            }
        }

        //If # found directly return preprocessor directive
        if(buffer[pos] == '#')
        {
            //Get the entire line content and store into lexeme member in structure
            while(buffer[pos] != '\n' && buffer[pos] != '\0')
            {
                generate_token.lexeme[i++] = buffer[pos++];
            }

            generate_token.lexeme[i] = '\0';
            //Set the token type to preprocessor directive
            generate_token.type = PREPROCESSOR_DIRECTIVE;
            return generate_token;
        }
        
        //If space is found between the characters skip it 
        while(buffer[pos] == ' ')
        {
            pos++;
        }

        //If single comment skip it entirely
        if(buffer[pos] == '/' && buffer[pos+1] == '/')
        {
            pos = strlen(buffer);
            continue;
        }

        //If multi line comments end in different lines
        if(multi_line)
        {
            while(buffer[pos] != '\0')
            {
                if(buffer[pos] == '*' && buffer[pos+1] == '/')
                {
                    pos += 2;
                    multi_line = 0;
                    break;
                }
                pos++;
            }
            continue;
        }

        //If multi line comment ends in same line
        if(buffer[pos] == '/' && buffer[pos+1] == '*')
        {
            pos += 2;
            multi_line = 1;

            while(buffer[pos] != '\0')
            {
                if(buffer[pos] == '*' && buffer[pos+1] == '/')
                {
                    pos += 2;
                    multi_line = 0;
                    break;
                }
                pos++;
            }
            continue;
        }
        
        //If string opening is found
        if(buffer[pos] == '"')
        {
            //Copy it into lexeme member in structure
            generate_token.lexeme[i++] = buffer[pos++];

            //Copy till either string closing is found or till \n or till string terminator
            while(buffer[pos] != '"' && buffer[pos] != '\n' && buffer[pos] != '\0')
            {
                generate_token.lexeme[i++] = buffer[pos++];
            }

            //If string closing is found
            if(buffer[pos] == '"')
            {
                //Copy it into lexeme member in structure
                generate_token.lexeme[i++] = buffer[pos++];
                //Add string terminator in lexeme member in structure
                generate_token.lexeme[i] = '\0';
                //Set token type to string literal
                generate_token.type = STRING_LITERAL;
                return generate_token;  
            }

            //If string closing is not found return string lexical error
            generate_token.lexeme[i] = '\0';
            generate_token.type = STRING_LEXICAL_ERROR;
            return generate_token;
        }

        //If char literal opening is found
        if(buffer[pos] == '\'')
        {
            //Copy it into lexeme member in structure
            generate_token.lexeme[i++] = buffer[pos++];

            //Copy till either closing char literal or \n or string terminator is found
            while(buffer[pos] != '\'' && buffer[pos] != '\n' && buffer[pos] != '\0')
            {
                generate_token.lexeme[i++] = buffer[pos++];
            }

            //If closing char literal is found
            if(buffer[pos] == '\'')
            {
                //Copy it into lexeme member in structure
                generate_token.lexeme[i++] = buffer[pos++];
                //Add string terminator to lexeme member in structure
                generate_token.lexeme[i] = '\0';
                //Set token type to char literal
                generate_token.type = CHAR_LITERAL;
                return generate_token;  
            }

            //Character literal should contain only one character  
            if(i > 3)
            {
                generate_token.lexeme[i] = '\0';
                printf("Lexical Error [Line %d] : Character literal should contain only one chararcter -> %s\n",line,generate_token.lexeme);
                generate_token.type = UNKNOWN;
                return generate_token;
            }

            //If closing char literal not found then return char lexical error
            generate_token.lexeme[i] = '\0';
            generate_token.type = CHAR_LEXICAL_ERROR;
            return generate_token;
        }

        //Run a loop till buffer reaches string terminator of that particular line
        while(buffer[pos] != '\0')
        {
            //If space or tab or newline is found
            if(buffer[pos] == ' ' || buffer[pos] == '\t' || buffer[pos] == '\n')
            {
                //If some characters are present in the lexeme member in structure then process them first
                if(i != 0)
                {
                    //Move the position to next character
                    pos++;
                    //Add string terminator in lexeme member in structure
                    generate_token.lexeme[i] = '\0';
                    //Call categorize function and then return token 
                    categorizeToken(&generate_token);
                    return generate_token;
                }
                //If no characters are present in lexeme member in structure
                else
                {
                    //If nothing is present in buffer increment position and process next character
                    pos++;
                    continue;
                }
            }
            //If any operator or special character is found
            else if(isOperator(buffer[pos]) || isSpecialCharacter(buffer[pos]))
            {
                //If already some characters are present in lexeme member in structure process them first
                if(i > 0)
                {
                    //Add string terminator to lexeme member in structure
                    generate_token.lexeme[i] = '\0';
                    //Call categorize token function and return token
                    categorizeToken(&generate_token);
                    return generate_token;
                }
                //If no characters are present in lexeme member in structure
                else
                {
                    //Copy the operator or special character into lexeme member in structure
                    generate_token.lexeme[i++] = buffer[pos++];
                    //Add string terminator in lexeme member in structure
                    generate_token.lexeme[i] = '\0';
                    //Call categorize token function and return token
                    categorizeToken(&generate_token);
                    return generate_token;
                }
            }
            else
            {
                //Add the character to lexeme member in structure
                generate_token.lexeme[i++] = buffer[pos++];
            }
        }
    

        //If file pointer reaches EOF
        if(i != 0)
        {   
            //Add string terminator to lexeme member in structure
            generate_token.lexeme[i] = '\0';
            //Call categorize token function and return token
            categorizeToken(&generate_token);
            return generate_token;
        }
    }
}

void categorizeToken(Token* token)
{
    //Keyword
    if(isKeyword(token->lexeme))
    {
        token->type = KEYWORD;
    }
    //Operator
    else if(strlen(token->lexeme) == 1 && isOperator(token->lexeme[0]))
    {
        token->type = OPERATOR;
    }
    //Special character
    else if(strlen(token->lexeme) == 1 && isSpecialCharacter(token->lexeme[0]))
    {
        token->type = SPECIAL_CHARACTER;
    }
    //Constant
    else if(isConstant(token->lexeme))
    {
        token->type = CONSTANT;
    }
    //Identifier
    else if(isIdentifier(token->lexeme))
    {
        token->type = IDENTIFIER;
    }
    //Unknown
    else
    {
        token->type = UNKNOWN;
    }
}

int isKeyword(const char* str)
{
    //Check if given string is present in static keyword array
    for(int i=0;i<MAX_KEYWORDS;i++)
    {
        if(strcmp(keywords[i],str) == 0)
        {
            return SUCCESS;
        }
    }
    return FAILURE;
}

int isOperator(const char ch)
{
    //Check if given character is present in operators string literal
    if(strchr(operators,ch) != NULL)
    {
        return SUCCESS;
    }
    return FAILURE;
}

int isSpecialCharacter(const char ch)
{
    //Check if given character is present in special characters string literal
    if(strchr(specialCharacters,ch) != NULL)
    {
        return SUCCESS;
    }
    return FAILURE;
}

int isConstant(const char* str)
{
    int i = 0;

    //Check whether it is a constant between 0 to 9
    while(str[i] != '\0')
    {
        if(!(isdigit(str[i])))
        {
            return FAILURE;
        }
        i++;
    }
    return SUCCESS;
}

int isIdentifier(const char* str)
{
    //Check if it starts with either letters or underscore only
    if(!(isalpha(str[0]) || str[0] == '_'))
    {
        return FAILURE;
    }

    int i = 1;

    //Check whether entire string contains only letters numbers and underscore only
    while(str[i] != '\0')
    {
        if(!(isalnum(str[i]) || str[i] == '_'))
        {
            return FAILURE;
        }
        i++;
    }
    return SUCCESS;
}

const char* typeToString(TokenType type)
{
    //Based on enum return string
    switch(type)
    {
        case PREPROCESSOR_DIRECTIVE:
        return "Preprocessor directive";
        break;

        case KEYWORD:
        return "Keyword";
        break;

        case OPERATOR:
        return "Operator";
        break;

        case SPECIAL_CHARACTER:
        return "Special Character";
        break;

        case CONSTANT:
        return "Constant";
        break;

        case IDENTIFIER:
        return "Identifier";
        break;

        case CHAR_LITERAL:
        return "Char literal";
        break;

        case STRING_LITERAL:
        return "String literal";
        break;

        case END_OF_FILE:
        return "End of File";
        break;

        case UNKNOWN:
        return "Unknown";
        break;

        default:
        return "Unknown";
    }
}