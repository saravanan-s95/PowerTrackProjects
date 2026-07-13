#include <stdio.h>          // Standard input/output functions
#include <stdarg.h>         // Variable argument functions
#include "main.h"           // User-defined header

int line_no = 1;            // Global line number counter

int main(int argc, char *argv[])
{
    char lexme[100];        // Buffer to store extracted lexeme

    if (argc != 2)          // Check for correct number of command-line arguments
    {
        printf(RED BOLD "Invalid number of arguments\n" RESET);
        printf(YELLOW "Usage -> ./a.out <file.c>\n" RESET);
        return 0;
    }

    FILE *fp = fopen(argv[1], "r");      // Open input C source file

    if (fp == NULL)                     // Check if file exists
    {
        printf(RED BOLD "File : %s -> is not present in memory\n" RESET, argv[1]);
        return 0;
    }

    if (remove_comments(fp) == 0)       // Remove comments and generate process.txt
    {
        printf(RED BOLD "There is issue in removing comments from the file\n" RESET);
    }

    FILE *pp = fopen("process.c", "r");   // Open processed file

    if (pp == NULL)                         // Check if process file opened successfully
    {
        printf(RED BOLD "There is error opening process file\n" RESET);
        return 0;
    }

    printf("\n");

    // Print table header
    printf(MAGENTA BOLD "=====================================================================\n" RESET);
    printf(BLUE BOLD "%-8s" RESET
           YELLOW BOLD "%-25s" RESET
           GREEN BOLD "%-30s\n" RESET,
           "LINE", "LEXEME", "TOKEN / ERROR");
    printf(MAGENTA BOLD "=====================================================================\n" RESET);

    while (!feof(pp))                  // Read until end of processed file
    {
        get_next_lexme(pp, lexme);     // Extract next lexeme

        if (lexme[0] == '\0')          // Skip empty lexemes
        {
            continue;
        }

        int ret = check_error(lexme);  // Check whether lexeme contains any lexical error

        if (ret)
        {
            print_error(ret, lexme);   // Print corresponding error
            continue;
        }

        int type = classify_token(lexme); // Classify valid token

        print_type(type, lexme);          // Display token type
    }

    printf(MAGENTA BOLD "=====================================================================\n" RESET);

    rewind(pp);                 // Move file pointer to beginning

    check_brackets(pp);         // Check for bracket mismatch

    rewind(pp);                 // Reset file pointer (optional)

    fclose(pp);                 // Close processed file

    fclose(fp);                 // Close original input file

    return 0;                   // Program ends successfully
}