#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexicalanalyser.h"
char buffer[5000];
int main()
{
    printf("<----------------WELCOME TO LEXICAL ANALYSER------------------>");
    FILE *fptr = fopen("program.c", "r");  
    if (!fptr)
    {
        printf("File opening Failed\n");
        return 0;
    }
    int i = 0;
    char ch;
    while ((ch = fgetc(fptr)) != EOF && i < sizeof(buffer) - 1) // To avoid overflow of buffer we check this i<sizeof(buffer)-1
    {
        buffer[i++] = ch;
    }
    buffer[i]='\0';
    fclose(fptr);
    lexicalanalyser();
    
}