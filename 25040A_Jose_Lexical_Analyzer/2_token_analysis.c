#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "4_lexical.h"

extern const char* keywords[MAX_KEYWORDS];
extern const char* specialCharacters;
extern const char* operators;


//Function to read data from file (line-by-line)
int read_data_from_file_type_analysis(FILE* fptr)
{
    char ch = 0;
    char str[50];

    while(fscanf(fptr, " %[^\n]", str) > 0)     //Read data till '\n' and store to str
    {
        int length = strlen(str);
        if(length>0 && str[length-1] == '\r')   //If any '\r' present in str, replace with '\0'
        {
            str[length-1] = '\0';
        }
        fgetc(fptr);                            //To discard the '\n
        
        if(is_preprocessor_directive(str) == TRUE)      //Check whether the data read is pre-processor directive
        {
            printf("%-30s : %s\n", str, "Pre-processor directive");
        }
        else
        {
            separate_token(str);        //If not pre-processor directive, separate tokens and categorize (line-by-line)
        }
    }
}


//Function to check whether pre-processor directive
int is_preprocessor_directive(char* str)
{
    if(str[0] == '#')
    {
        return TRUE;
    }
    return FALSE;
}


//Function to separate toke based on space, special characters, operators etc
int separate_token(char* str)
{
    char token[50];     

    int i=0;
    int j=0;
    while(str[i]!='\0')
    {
        while(str[i]==' ' || str[i]=='\t')      //Discard any leading spaces or tabs
        {
            i++;
        }

        if(str[i]=='\0')        //Stop when reaching the end of the string (ie., after completing one line)
        {
            break;
        }

        j=0;
        
        if(str[i] == '"')       //Case when str starts with " 
        {
            token[j] = str[i];
            j++;
            i++;
            while(str[i]!='\0' && str[i]!='"')  //store all characters till '\0' or next " into the token.
            {
                token[j] = str[i];
                j++;
                i++;
            }
            if(str[i] == '"')       //Finally, store the ending " also to the token
            {
                token[j] = str[i];
                j++;
                i++;
            }
        }

        //If the string contains brackets, comma or semi-colon, store to token
        else if(str[i]=='(' || str[i]==')' || str[i]=='{' || str[i]=='}' || str[i]==';' || str[i]==',')
        {
            token[j] = str[i];
            j++;
            i++;
        }

        //If the string contains the operatore, store to token
        else if(str[i]=='+' || str[i]=='-' || str[i]=='*' || str[i]=='/' || str[i]=='%' ||
                str[i]=='=' || str[i]=='!' || str[i]=='<' || str[i]=='>' || str[i]=='|' || str[i]=='&')
        {
            token[j] = str[i];
            j++;
            i++;
        }

        //If the string elements are characters, then store each character to the token till the delimitter is reached
        //(The delimiters are: space, operators, special characters, \t or  '\0)
        else
        {
            while(is_token_delimiter(str[i]) == FALSE)
            {
                token[j] = str[i];
                j++;
                i++;
            }
        }

        token[j] = '\0';            //Append the token with '\0' at the end

        categorizeToken(token);     //Categorize and print each token details.

    }
     
    return SUCCESS;
}


//Functio to categorize and print each token details
void categorizeToken(char* token)
{
    if(isKeyword(token) == KEYWORD)                     //Check whether the token is keyword
    {
        printf("%-30s : %s\n", token, "KEYWORD");
    }

    else if(isOperator(token) == OPERATOR)              //Check whether the token is operator
    {
        printf("%-30s : %s\n", token, "OPERATOR");
    }

    else if(isSpecialCharacter(token) == SPECIAL_CHARACTER)     //Check whether the token is special character
    {
        if(token[0] == ';')                                     //If it is semicolon, prints end-of-statement
        {
            printf("%-30s : %s\n", token, "END OF STATEMENT");
        }
        else                                                    //Otherwise, Print special-character
        {
            printf("%-30s : %s\n", token, "SPECIAL CHARACTER");
        }
    }

    else if(isConstant(token) == CONSTANT)              //Check whether the token is constant/literal
    {
        printf("%-30s : %s\n", token, "CONSTANT/LITERAL");
    }

    else                    //Identifer case
    {
        printf("%-30s : %s\n", token, "IDENTIFIER");
    }
}


//Function to find delimiters (For separating token)
int is_token_delimiter(char ch)
{
    if( ch=='\0'|| ch==' ' || ch=='"' || ch=='\t' || 
        ch=='(' || ch==')' || ch=='{' || ch=='}'  ||
        ch==';' || ch==',' || ch=='+' || ch=='-'  || 
        ch=='*' || ch=='/' || ch=='%' || ch=='='  ||
        ch=='<' || ch=='>' || ch=='!' || ch=='|'  ||
        ch=='&')
    {
        return TRUE;
    }

    return FALSE;
}


//Function to find whether the token is a keyword
int isKeyword(const char* token)
{
    for(int i=0; keywords[i]!=NULL; i++)
    {
        if(strcmp(keywords[i], token) == 0)
        {
            return KEYWORD;
        }
    }
    return UNKNOWN;
}


//Function to find whether the token is an operator
int isOperator(const char* token)
{
    int length = strlen(token);

    if(length == 1)
    {
        for(int i=0; operators[i]!='\0'; i++)
        {
            if(operators[i] == token[0])
            {
                return OPERATOR;
            }
        }
    }
    return UNKNOWN;
}


//Function to find whether the token is a special character
int isSpecialCharacter(const char* token)
{
    int length = strlen(token);

    if(length == 1)
    {
        for(int i=0; specialCharacters[i]!='\0'; i++)
        {
            if(specialCharacters[i] == token[0])
            {
                return SPECIAL_CHARACTER;
            }
        }
    }
    return UNKNOWN;
}


//Function to find whether the token is a constat
int isConstant(const char* token)
{
    int length = strlen(token);

    if(token[0] == '"' && token[length-1] == '"')       //String case: If the token starts and ends with " then it is a literal
    {
        return CONSTANT;
    }

    if(token[0]=='0')               //  The case of hexadecimal values
    {
        if(token[1]=='X' || token[1]=='x')
        {
            for(int i=2; i<length; i++)
            {
                char ch = token[i];
                if(isdigit(ch) || (ch>=65 && ch<=70) || (ch>=97 && ch<=102))
                {
                    return CONSTANT;
                }
            }
        }
    }

    if((length==1) && (isdigit(token[0])))      //The case of 1-digit values
    {
        return CONSTANT;
    }
    else if(length > 1)         //The case of numbers with more than one digits
    {
        if(atoi(token) != 0)
        {
            return CONSTANT;
        }
    }

    return UNKNOWN;
}
