#include<stdio.h>      
#include<stdlib.h>     
#include<string.h>     
#include"main.h"       

// Define an enum to represent operation types
typedef enum 
{
    operation_invalid,   // Invalid or unknown operation
    operation_view,      // View MP3 tags
    operation_edit       // Edit MP3 tags
}OperationType;

// Function to check the operation type based on command-line arguments
OperationType check_operation_type(int argc, char *argv[])
{
    if(argc < 2)   // If no sufficient arguments are passed
    {
        printf("ERROR : Insufficient arguments.\n");
        printf("USAGE : ./a.out -v <file.mp3>(SAMPLE.mp3)\n");
        return operation_invalid;   // Return invalid operation
    }

    if(strcmp(argv[1], "-v") == 0)   // If first argument is -v (view mode)
    {
        if(argc != 3)   // Expect exactly 3 arguments
        {
            printf("ERROR : For view option at least 3 arguments are needed.\n");
            return operation_invalid;
        }
        return operation_view;   // Return view operation type
    }
    else if(strcmp(argv[1], "-e") == 0)   // If first argument is -e (edit mode)
    {
        if(argc != 5)   // Expect exactly 5 arguments
        {
            printf("ERROR : For edit option at least 5 arguments are needed.\n");
            return operation_invalid;
        }
        return operation_edit;   // Return edit operation type
    }
    else   // If option is neither -v nor -e
    {
        printf("ERROR : Unknown option is provided.\n");
        return operation_invalid;   // Return invalid operation
    }
}

// Main function (program execution starts here)
int main(int argc, char* argv[])
{
    // Check what type of operation user wants
    OperationType op_type = check_operation_type(argc, argv);

    if(op_type == operation_view)   // If operation is view
    {
        view_mp3_tags(argv[2]);     // Call function to view tags, pass filename
    }
    else if(op_type == operation_edit)   // If operation is edit
    {
        const char *frame = option_frame(argv[2]);   // Map option (-t, -A, etc.) to frame ID
        if(!frame)   // If invalid option was passed
        {
            printf("Error : Invalid option.\n Usage of options are [-t] [-A] [-a] [-y] [-C] [-c]\n");
            return 1;   // Exit program with error
        }
        edit_mp3_tags(frame, argv[3], argv[4]);   // Call function to edit tag
    }
    else   // If invalid operation type
    {
        // Show correct usage instructions
        printf("Usage : View : ./a.out -v SAMPLE.mp3\n Edit : ./a.out -e -t [NewTitle] SAMPLE.mp3\n");
        operation_invalid;   
        return 0;            
    }
}
