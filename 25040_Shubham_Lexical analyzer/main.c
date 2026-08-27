#if 0
Project Name:Lexical analyzer
Name: Shubham Maroti Jagtap
Date: 05/07/2026
description:A Lexical Analyzer (Lexer) developed in C that scans source code and converts it into a stream of tokens. 
It identifies keywords, identifiers, operators, constants, delimiters, and literals while ignoring whitespace and comments, demonstrating the first phase of compiler design.
#endif

#include <stdio.h>
#include "lexer.h"
int print_token(Token * token);
int main(int argc,char *argv[]) //collecting the command line argument
{
    if(validate(argv[1],argc)==0)
    {
        return 0;
    }
    if(initializeLexer(argv[1])==0) //open the file
    {
        return 0;
    } 
    Token token;
    while(1)
    {
        token = getNextToken(); 
        if(token.type == ERROR)
        {
            return 0;
        }
        else if(token.type== SKIP)
        {
            continue;
        }
        if(strcmp(token.lexeme,"EOF")==0)
        {
            break;
        }
        printToken(token); //printing the tokens
    }
    return 0;
}
