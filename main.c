#include "main.h"

int main(int argc, char*argv[])
{
    FILE *fptr; //file pointer.
    if(argc!=2) //check argument count.
    {
        printf(RED "usage --> ./a.out <filename>\n" RESET);
        return FAILURE;
    }
    fptr=fopen(argv[1],"r");    //open file in read mode.
    if(fptr==NULL)  //if file not found return.
    {
        printf(RED "Error opening file\n" RESET);
        return FAILURE;
    }
    printf("\n");
    printf(ORANGE "LINE \t\t TOKEN \t\t TYPE\n" RESET);
    printf("------------------------------------------\n");
    lexical_analyzer(fptr);     //call function.
    fclose(fptr);   //close file pointer.
    return 0;
}