#include<stdio.h>
#include <string.h>
#include "lexer.h"
int validate(char *str,int count)
{
    if(count!=2)
    {
        printf(RED"\nError: Check number of arguments\n\n"RESET);
        return 0;
    }
    char *ptr=strrchr(str,'.');
    if(ptr==NULL)
    {
        printf(RED"\nError: Invalid file extention %s\n\n"RESET,str);
        return 0;
    }
    if((strcmp(ptr,".txt")!=0 && strcmp(ptr,".c")!=0))
    {
        
         printf(RED"\nError: Invalid file extention %s\n\n"RESET,str);
        return 0;
    }
    return 1;
}