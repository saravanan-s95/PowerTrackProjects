#include <stdio.h>
#include "myheader.h"

/*---------------------------------------------------
  Multi Line Comment Test
  Source To HTML Verification
---------------------------------------------------*/

// Single Line Comment Test

#define MAX_COUNT 100
#define PI_VALUE 3.14

const int global_var = 50;

int main()
{
    /* Data Type Keywords */
    int num = 10;
    float pi = 3.14;
    char ch = 'A';
    double d = 12.3456;
    long l = 1000;
    short s = 5;

    /* Hex and Decimal Numbers */
    int hex1 = 0x12;
    int hex2 = 0XAB;
    int dec = 12345;

    /* String Test */
    printf("Hello World\n");
    printf("Source To HTML Test\n");

    /* ASCII Character Test */
    char c1 = 'Z';
    char c2 = '9';
    char c3 = '*';

    /* Non Data Keywords Test */
    if(num < MAX_COUNT)
    {
        for(int i = 0; i < 5; i++)
        {
            printf("Value = %d\n", i);
        }
    }
    else
    {
        return 0;
    }

    while(num > 0)
    {
        num--;
    }

    switch(num)
    {
        case 0:
            break;

        default:
            break;
    }

    do
    {
        num++;
    }while(num < 5);

    sizeof(num);

    return 0;
}