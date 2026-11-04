#include <stdio.h>
#include "lexical.h"

int main(int argc, char *argv[])
{
    Lexer lexer;
    Token token;

    if (argc != 2)
    {
        printf("Usage: %s input.c\n", argv[0]);
        return 1;
    }

    if (!lexer_init(&lexer, argv[1]))
    {
        perror("File opening failed");
        return 1;
    }

    printf("\nLEXICAL ANALYZER\n\n");
    printf("%-8s %-18s %s\n", "LINE", "TOKEN TYPE", "LEXEME");
    printf("-----------------------------------------------\n");

    do
    {
        token = get_next_token(&lexer);

        if (token.type != END_OF_FILE)
        {
            printf("%-8d %-18s %s\n",
                   token.line,
                   get_token_name(token.type),
                   token.lexeme);
        }

    } while (token.type != END_OF_FILE);

    lexer_destroy(&lexer);

    printf("\nLexical analysis completed.\n");

    return 0;
}
