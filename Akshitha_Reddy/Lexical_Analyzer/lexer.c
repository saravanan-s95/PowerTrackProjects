#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include "lexer.h"

/* Array containing data type keywords */
char *datakeywords[]={"int","float","char","double","short",
    "long","signed","unsigned","void"};

/* Array containing non-data type keywords */
char *nonDatakeywords[]={"goto","return","continue","break","if",
    "for","else","while","do","switch","case","default","sizeof"}; 

/* Check whether the given token is a data type keyword */
int isDataKeyword(char *str)
{
    //to calculate no.of elements in array
    int n=sizeof(datakeywords)/sizeof(datakeywords[0]);  
    for(int i=0;i<n;i++)
    {
        if(strcmp(str,datakeywords[i])==0)
        {
            return 1;
        }
    }
    return 0;
}
/* Check whether the given token is a non-data type keyword */
int isnonDataKeyword(char *str)
{
    int n=sizeof(nonDatakeywords)/sizeof(nonDatakeywords[0]);
    for(int i=0;i<n;i++)
    {
        if(strcmp(str,nonDatakeywords[i])==0)
        {
            return 1;
        }
    }
    return 0;
}
/* Check whether the character is an operator */
int isOperator(char ch)
{
    return(ch=='+' || ch=='-' || ch=='*' || ch=='/' || ch=='%' || ch=='=' ||
           ch=='<' || ch=='>');
}
/* Check whether the character is an special character */
int isSpclChar(char ch)
{
    return(ch=='(' || ch==')' || ch=='{' || ch=='}' || ch=='[' || ch==']' ||
           ch==';' || ch==',');
}
/* Check whether the token is an integer literal */
int isInteger(char *str)
{
    int i;

    for(i=0; str[i]!='\0'; i++)
    {
        if(!isdigit(str[i]))
            return 0;
    }

    return 1;
}
int isFloat(char *str)
{
    int dot = 0;  //decimal point

    for(int i=0; str[i]!='\0'; i++)
    {
        if(str[i] == '.')
            dot++;
        else if(!isdigit(str[i]))
            return 0;
    }
     /* Valid float should contain exactly one decimal point */
    return (dot==1);
}



