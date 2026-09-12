#include <stdio.h>
#include <string.h>
#include "4_lexical.h"

const char* keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", 
    "struct", "auto", "long", "case", "enum", "typedef", "register", "extern", "union",
    "short", "unsigned", "signed", "goto", "volatile", "static", NULL
};

const char* operators = "+-*/%=!<>|&";

const char* specialCharacters = ",;{}()[]";


int main(int argc, char* argv[])
{
    if(argc < 2)        //Validating argument
    {
        printf("Invalid no. of arguments\n");
        return -1;
    }

    if(validate_filename(argv[1]) == FAILURE)       //Validate filename (input file)
    {
        printf("Filename validation Failure\n");
        return FAILURE;
    }

    FILE* fptr = fopen(argv[1], "r");       //Open the file in read mode
    if(fptr == NULL)
    {
        perror("fopen");
        return FAILURE;
    }

    read_data_from_file_type_analysis(fptr);    //Read data from file and categorize them

    fseek(fptr, 0, SEEK_SET);           //Rewind file pointer to the beginning of file - for error checking

    printf("\n\nERRORS\n");
    read_data_from_file_find_error(fptr);       //Read data from file for error checking

    return 0;
}


//Function to validate the input filename
int validate_filename(char* file)
{
    char* ret = strstr(file, ".c");
    if(ret == NULL)
    {
        printf(".c extension not found\n");
        return FAILURE;
    }

    if(strcmp(ret, ".c") != 0)      //Check for .c extension
    {
        printf("Filename has Invalid extension\n");
        return FAILURE;
    }

    return SUCCESS;
}
