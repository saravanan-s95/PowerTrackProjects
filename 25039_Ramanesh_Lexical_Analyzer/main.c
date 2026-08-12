// Name : Ramanesh M
// Batch : 25039 - 003
// Date of completion : 04/07/2026


#include<stdio.h> 
#include"lexer.h" 


int main(int argc, char* argv[]) // Main function where execution starts
{
    if(argc != 2) // Check whether the source file name is given
    {
        printf("Usage: ./lexer <source_file>\n"); // Show the correct way to run the program
        return 1; // Exit if the argument is missing
    }

    FILE *fp = fopen(argv[1],"r"); // Open the source file in read mode

    if(fp == NULL) // Check whether the file opened successfully
    {
        printf("Unable to open file\n"); // Print an error message if the file cannot be opened
        return 1; // Exit the program
    }

    printf("%-20s %-40s\n","LEXEME","TOKEN"); // Print the table heading
    printf("\n"); // Leave one empty line for better readability

    tokenize(fp); // Start reading and identifying tokens

    displayErrors(); // Display lexical errors if present

    fclose(fp); // Close the opened file

    return 0; // End the program successfully

}
