#include <stdio.h>
#include "analyzer.h"

int main(int argc, char *argv[])
{
    Lexer FILEINFO;
    Token token;

    // Step 1: Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: %s <source.c>\n", argv[0]);
        return 1;
    }

    // Step 2: Validate file format
    if (!check_file_formate(argv[1]))
    {
        printf("Invalid file format. Please provide a .c file.\n");
        return 1;
    }

    // Step 3: Open source file
    FILEINFO.source = fopen(argv[1], "r");

    if (FILEINFO.source == NULL)
    {
        perror("Unable to open source file");
        return 1;
    }

    // Step 4: Initialize lexer
    initLexer(&FILEINFO, FILEINFO.source);

    // Step 5: Generate tokens
    while (1)
    {
        token = getNextToken(&FILEINFO);

        if (token.type == TOKEN_EOF)
        {
            break;
        }

        // Step 6: Display token
        printToken(token);
    }

    // Step 7: Close source file
    fclose(FILEINFO.source);

    return 0;
}