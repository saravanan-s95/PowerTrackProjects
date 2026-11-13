/*
Name        : Sheeba Insily R
Project     : Lexical Analyzer
Description : This project performs lexical analysis of a C source file.
              It identifies and displays keywords, identifiers, constants,
              operators, special characters, strings, characters and headers.
*/


#include "lexer.h"

int main(int argc, char *argv[])
{
    FILE *fp;

    /* Check command-line argument */
    if (argc != 2)
    {
        printf("Usage: %s <input_file.c>\n", argv[0]);
        return 1;
    }

    /* Open input C file */
    fp = fopen(argv[1], "r");

    if (fp == NULL)
    {
        printf("Error: Unable to open file %s\n", argv[1]);
        return 1;
    }

    printf("Lexical Analysis of: %s\n\n", argv[1]);

    /* Start lexical analysis */
    lexical_analyzer(fp);

    /* Close file */
    fclose(fp);

    return 0;
}