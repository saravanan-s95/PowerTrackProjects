/*
Name: Kavyashree.K
Date:18/07/2026
Description: The project Lexical Analyzer is the first phase of a compiler that scans a C source file character by character \
and converts it into meaningful tokens while performing basic lexical error detection.*/



#include "lexer.h"        //header file with function declarations,arrays,macros

int main(int argc, char *argv[])      //command line argumemts count and array of strings
{
    if(argc != 2)                 //if ./a.out 
    {
        printf("Usage: %s <source_file.c>\n", argv[0]);    //error msg
        return 1;
    }

    FILE *fin = fopen(argv[1], "r");           //open sample.c file to analyse

    if(fin == NULL)
    {
        printf("Error: Unable to open file %s\n", argv[1]);  //if file does not exist
        return 1;
    }

    lexical_analysis(fin);      //function call

    fclose(fin);   //close the file after operation

    return 0;
}