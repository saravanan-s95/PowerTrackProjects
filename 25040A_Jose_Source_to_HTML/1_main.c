#include <stdio.h>
#include "main.h"

char input_file[50];
char output_file[50];

int line_print_mode = 0;

FILE* dst_fp;
FILE* src_fp;

int main(int argc, char* argv[])
{
    if(validate_inputs(argc, argv) == FAILURE)        //Validate input
    {
        return FAILURE;
    }
    printf("Validations success\n");

    printf("Input file: %s\n", input_file);
    printf("Output file: %s\n", output_file);

    src_fp = fopen(input_file, "r");        //Open input file in read mode
    if(src_fp == NULL)
    {
        printf("Input file opening failed\n");
        return FAILURE;
    }

    dst_fp = fopen(output_file, "w");       //Open output file in write mode
    if(dst_fp == NULL)
    {
        printf("Output file opening failed\n");
        return FAILURE;
    }

    html_begin(dst_fp);             //To print the opening tags to the html file

    if(line_print_mode == 0)        //If user didnot pass "-n", html file donot require line numbers
    {
        get_character_from_file();
    }
    else if(line_print_mode == 1)   //If user passed "-n", html file require line numbers
    {
        get_character_from_file_number();
    }

    html_end(dst_fp);               //To print the closing tags to the html file

    printf("\nOutput html file Generated\n");

    fclose(src_fp);     //Close input file
    fclose(dst_fp);     //Close output file

    return SUCCESS;
}
