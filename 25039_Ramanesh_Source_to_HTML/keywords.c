#include<stdio.h>
#include<string.h>
#include"source2html.h"

char *keywords[]={
"auto","break","case","char","const","continue","default","do","double","else","enum","extern","float","for","goto","if","int","long","register","return",
"short","signed","sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while",NULL
};

int isKeyword(char *word)
{
    int i=0;

    while(keywords[i]!=NULL)
    {
        if(strcmp(word,keywords[i])==0)
            return 1;
        i++;
    }

    return 0;
}