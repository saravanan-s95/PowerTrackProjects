#include <stdio.h>
#include "lexer.h"
FILE *fp;
int initializeLexer(const char* filename)
{
    // FILE *fp;
     fp=fopen(filename,"r");
     if(fp==NULL)
     {
        //fprintf(stderr,"file is not open\n");
        perror("fopen");
        return 0;
     }
     return 1;
     //getNextToken();
    
}
