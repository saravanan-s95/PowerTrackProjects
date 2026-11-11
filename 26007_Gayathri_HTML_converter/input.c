#include <stdio.h>

#define CONVERSION 0       //define a constant name CONVERSION with value 0

int main()                 //main function
{
    char system = 'a';     //declare a character variable 'system ' initialize with a

    if(system == 'a')      //check if system == a
    {
        printf("Hello world\n");  //printf hello world
    } 
    else if(system == 'b')     //check if system == B
    {
        printf("World\n");      //printf world
    }
    else                        //if 'system' is neither 'a' and 'b'
    {
        printf("Hello");         //pirnt hello
    }
    return 0;            
}