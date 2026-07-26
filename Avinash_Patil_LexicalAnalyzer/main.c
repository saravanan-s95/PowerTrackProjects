#include <stdio.h>
#include "lexer.h"
#include <string.h>
#include <unistd.h>

extern FILE *ptr;

static const char* type[] = {
    "KEYWORD", "IDENTIFIER","OPERATOR","SPECIAL_CHARACTER",
    "NUMERIC_CONSTANT","CHARACTER_CONSTANT","HEXADECIMAL",
    "BINARY","OCTAL","STRING LITERAL"
};

/* Brace lookup strings */
char *braces = "({[]})";
char *open_braces = "({[";
char *close_braces = ")}]";

int main(int argc, char *argv[]) 
{
    /* Validate command line arguments */
    if(argc != 2){
        printf("Error: Invalid no of arguments\n");
        return -1;
    }

    /* Validate input file extension */
    if(strstr(argv[1], ".c") == NULL){
        printf("Error: Invalid file type\n");
        return -1;
    }

    /* Initialize lexer with source file */
    initializeLexer(argv[1]);

    if(ptr == NULL){
        printf("ERROR : File opening failed\n");
        return -1;
    }
    
    Token token;

    char stack[20];     // Stack to store opening braces
    int top = -1;

    /* Process tokens until error or EOF */
    while ((token = getNextToken()).type != UNKNOWN) {
        /* Check whether token is a brace */
        if(strstr(braces, token.lexeme) != NULL){
            /* Push opening braces */
            if(strchr(open_braces, token.lexeme[0]) != NULL){
                stack[++top] = token.lexeme[0];
            }

            /* Validate closing braces */
            else if(strchr(close_braces, token.lexeme[0]) != NULL){
                /* Closing brace without opening brace */
                if(top == -1){
                    printf("ERROR : Missing open braces\n");
                    return -1;
                }
                /* Check matching brace pair */
                if(stack[top] == '(' && token.lexeme[0] == ')')
                    top--;
                else if(stack[top] == '{' && token.lexeme[0] == '}')
                    top--;
                else if(stack[top] == '[' && token.lexeme[0] == ']')
                    top--;

                /* Brace mismatch */
                else{
                    if(stack[top] == '(')
                        printf("ERROR : Missing ')'\n");
                    else if(stack[top] == '{')
                        printf("ERROR : Missing '}'\n");
                    else if(stack[top] == '[')
                        printf("ERROR : Missing ']'\n");
                    return -1;
                }
            }
        }
        /* Display valid token */
        printf("%-25s : %s\n", type[token.type], token.lexeme);
    }
    
    /* Display lexical error */
    if(strchr(token.lexeme, EOF) == NULL)
        printf("ERROR : %s\n", token.lexeme);

    /* Check remaining unclosed braces */
    if(top >= 0){
        if(stack[top] == '('){
            printf("ERROR : Missing ')'\n");
        }
        else if(stack[top] == '{'){
            printf("ERROR : Missing '}'\n");
        }
        else if(stack[top] == '['){
            printf("ERROR : Missing ']'\n");
        }
    }
    return 0;
}