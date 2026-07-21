#include <stdio.h>
#include "lexer.h"

int main(int argc , char *argv[])
{
    if(argc != 2)
    {
        printf("Invalid arguments\n");
        printf("To Analyze tokens use:   ./a.out <file_name.c>\n");
        printf("Example: ./a.out sample.c\n\n");
        return 1;
    }
    
    Lexer lexer;
    if(read_and_validate_args(argv , &lexer) == e_success){
        if(analyze_file(&lexer) != e_failure){
            printf("==========================================================\n");
            printf("LEXICAL ANALYSIS REPORT\n");
            printf("==========================================================\n");
            printf("%-5s | %-20s | %-20s\n", "LINE" , "TOKEN" , "TYPE");
            printf("----------------------------------------------------------\n");
            display_tokens(&lexer);
        }
    }else
        printf("Failed to Analyze\n");
}