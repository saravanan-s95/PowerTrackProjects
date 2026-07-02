/**
 * Student Name: Shreyas Manoj Kotian
 * Batch Number: 25031
 * Project Title: Lexical Analyzer
 * Project Description: A simple lexical analyzer for C source files
 */

#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "types.h"

int main(int argc, char* argv[]) {
    char filename[100];

    // Get filename from command line or ask user
    if (argc > 1) {
        strcpy(filename, argv[1]);
    } else {
        printf("Enter C source file name: ");
        scanf("%s", filename);
    }

    char* ch = strrchr(filename, '.');
    if (!ch || strcmp(ch, ".c") != 0) {
        printf("ERROR: Invalid file type!\n");
        printf("═══════════════════════════════════════\n");
        printf("File '%s' does not have a .c extension.\n", filename);
        printf("Please provide a valid C source file (.c).\n");
        printf("═══════════════════════════════════════\n");
        return 1;
    }

    // Analyze the file
    analyze_file(filename);

    return 0;
}