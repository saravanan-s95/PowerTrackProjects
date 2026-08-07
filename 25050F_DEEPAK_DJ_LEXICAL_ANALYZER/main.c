
#include "lexer.h"
#include<stdio.h>
#include<string.h>
#include<ctype.h>
int main(int argc, char *argv[]) {
    if(argc <=1 )
    {
        printf("\tEnter the correct command line argument\n");
        printf("\tEx : ./lexical_analyzer filename.c");
        return 0;
    }
    else
    {
      if(strstr(argv[1],".c")== NULL){
         printf("\tEnter the correct .c file name\n");
         return 0;
      }
      FILE *fp;
      if((fp = fopen(argv[1],"r"))==NULL){
        printf("\t Unable to open file bro \n");
        return 0;
      }
      else
      {
        printf("\t FILE OPENEND SUCCESSFULLY \n");
      }
       initializeLexer(fp);
    }
    return 0;
}
