
#include <stdio.h>
#include "lexical_analyzer.h"


int main(int argc, char *argv[])
{
    FILE *fp;
    if (argc != 2)
    {
        printf("Usage: ./a.out <filename.c>\n");
        return 1;
    }

    fp = fopen(argv[1],"r");

    if(fp == NULL)
    {
        printf("Open %s: Failure\n",argv[1]);
        return 1;
    }

    printf("Open %s: Success\n",argv[1]);
    printf("Parsing %s Started\n",argv[1]);

    lexical_analyze(fp);

    printf("Parsing: %s: Done\n",argv[1]);

   

    fclose(fp);

    return 0;
}
