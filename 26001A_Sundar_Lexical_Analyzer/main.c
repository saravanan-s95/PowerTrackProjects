#include <stdio.h>
#include "lexer.h"

int main(int argc, char *argv[]) {
    //If no file is passed
    if(argc < 2)
    {
        printf("USUAGE : ./a.out <.c file>\n");
        return FAILURE;
    }

    //Initialize lexer
    if(!(initializeLexer(argv[1])))
    {
        return FAILURE;
    }
    printf("Open : %s : Success\n",argv[1]);
    printf("Parsing : %s : Started\n\n\n",argv[1]);

    //Initialize structure
    Token token;
    int flag = 0;

    //Get tokens
    while ((token = getNextToken()).type != UNKNOWN) {
        //If EOF then break out of loop
        if(token.type == END_OF_FILE)
        {
            flag = 1;
            break;
        }
        //If string literal lexical error print error and break out of loop
        else if(token.type == STRING_LEXICAL_ERROR)
        {
            extern int line;
            printf("Lexical Error [Line %d] : Unterminated string literal ->  %s\n",line,token.lexeme);
            break;
        }
        //If char literal lexical error print error and break out of loop
        else if(token.type == CHAR_LEXICAL_ERROR)
        {
            extern int line;
            printf("Lexical Error [Line %d] : Unterminated character literal -> %s\n",line,token.lexeme);
            break;
        }

        //Print token type and the respective string or character
        printf("%-12s: %s\n", typeToString(token.type), token.lexeme);
    }

    if(flag)
    {
        //Success
        printf("\nParsing : %s : Done\n",argv[1]);
    }
    else
    {
        //Error
        printf("\nParsing : %s : Error\n",argv[1]);
    }

    return 0;
}
