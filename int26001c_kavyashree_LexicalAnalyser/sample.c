#include <stdio.h>

#define MAX 100

// This is a single line comment

/*
   This is a
   multi-line comment
*/

int main()
{
    int a = 10;
    float b = 12.5;
    char ch = 'A';
    double pi = 3.14159;

    int arr[5];

    printf("Hello World\n");

    if(a > 5)
    {
        b = b + 1.5;
    }
    else
    {
        b = 0;
    }

    for(int i = 0; i < 5; i++)
    {
        arr[i] = i;
    }

    return 0;
}