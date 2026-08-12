//TEST file to check all the functins 

#include <stdio.h>

#define MAX 100
#define PI 3.14

// Single line comment

/*
   Multi line comment
   Testing Source To HTML
*/

int global = 50;

int main()
{
    int a = 10;
    int b = 0x1A;
    float f = 12.5;
    double d = 25.75;
    char ch = 'A';
    char star = '*';

    printf("Hello World\n");
    printf("Value = %d\n", a);

    if (a > 5 && b < 50)
    {
        a = a + 1;
    }
    else
    {
        a = 0;
    }

    while (a < 20)
    {
        a++;
    }

    for (int i = 0; i < 5; i++)
    {
        printf("i = %d\n", i);
    }

    switch (a)
    {
        case 1:
            break;

        default:
            break;
    }

    return 0;
}
