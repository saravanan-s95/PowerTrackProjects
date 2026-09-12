#include <stdio.h>
#include <string.h>

#include "main.h"


//Function to validate input 
int validate_inputs(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Invalid no.of inputs\n");
        printf("\nThe input format is:\n");
        printf("For generating html file without line number:\t./a.out <input_file_name.c>  [output_file_name.html]\n");
        printf("For generating html file with line number:\t./a.out <input_file_name.c>  [output_file_name.html]  <-n>\n");
        return FAILURE;
    }

    else if(argc == 2)      //Inut filename passed; Output file name not passed
    {
        if(validate_input_filename(argv[1]) == FAILURE)
        {
            return FAILURE;
        }

        strcpy(input_file, argv[1]);            //Store the input filename to string "input_file"
        strcpy(output_file, "output.html");     //Take default filename for output file
    }

    else if((argc == 3) && (strcmp(argv[2], "-n")!=0))      //Both Inut filename and Output filename passed, -n not passed
    {
        if(validate_input_filename(argv[1]) == FAILURE)
        {
            return FAILURE;
        }
        strcpy(input_file, argv[1]);            //Store the input filename to string "input_file"

        if(validate_output_filename(argv[2]) == FAILURE)
        {
            return FAILURE;
        }
        strcpy(output_file, argv[2]);           //Store the output filename to string "output_file"
    }

    else if((argc == 3) && (strcmp(argv[2], "-n")==0))      //Inut filename passed, Output filename not passed, 3rd argument is -n
    {        
        if(validate_input_filename(argv[1]) == FAILURE)
        {
            return FAILURE;
        }
        strcpy(input_file, argv[1]);            //Store the input filename to string "input_file"

        strcpy(output_file, "output.html");     //Take default filename for output file

        line_print_mode = 1;                    //The output file must contain linenumber
        printf("Output file will contain line number\n");
    }

    else if(argc == 4  &&  (strcmp(argv[3], "-n")==0) )      //Both Inut filename and Output filename passed; Linenumber print mode
    {
        if(validate_input_filename(argv[1]) == FAILURE)
        {
            return FAILURE;
        }
        strcpy(input_file, argv[1]);            //Store the input filename to string "input_file"

        if(validate_output_filename(argv[2]) == FAILURE)
        {
            return FAILURE;
        }
        strcpy(output_file, argv[2]);           //Store the output filename to string "output_file"

        line_print_mode = 1;                    //The output file must contain linenumber
        printf("Output file will contain line number\n");
    }

    else
    {
        printf("Invalid command line arguments.\n");
        printf("\nThe input format is:\n");
        printf("For generating html file without line number:\t./a.out <input_file_name.c>  [output_file_name.html]\n");
        printf("For generating html file with line number:\t./a.out <input_file_name.c>  [output_file_name.html]  <-n>\n");
        return FAILURE;
    }

    return SUCCESS;
}


//Function to validate input file name
int validate_input_filename(char* input_file_name)
{
    char* ret = strstr(input_file_name, ".c");
    if(ret == NULL)
    {
        printf(".c extension not found in input filename\n");
        return FAILURE;
    }

    if(strcmp(ret, ".c") != 0)      //Check for .c extension
    {
        printf("Input filename has Invalid extension\n");
        return FAILURE;
    }

    return SUCCESS;
}


//Function to validate output file name
int validate_output_filename(char* output_file_name)
{
    char* ret = strstr(output_file_name, ".html");
    if(ret == NULL)
    {
        printf(".html extension not found in output filename\n");
        return FAILURE;
    }

    if(strcmp(ret, ".html") != 0)      //Check for .html extension
    {
        printf("Output filename has Invalid extension\n");
        return FAILURE;
    }

    return SUCCESS;
}