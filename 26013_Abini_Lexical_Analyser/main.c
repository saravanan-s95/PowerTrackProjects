#include <stdio.h>
#include "lexer.h"

int main()
{
    FILE *fp;

    fp = fopen("input.c", "r");

    if (fp == NULL)
    {
        printf("Error: Unable to open input.c\n");
        return 1;
    }

    analyse(fp);

    fclose(fp);

    return 0;
}
