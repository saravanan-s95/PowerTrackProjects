#include <stdio.h>

int main()
{
    int a = 10;
    int b = 20;

    a = b;
    a += 5;
    a -= 2;
    a *= 3;
    a /= 2;
    a %= 2;

    if (a == b)
    {
        a++;
    }

    if (a != b)
    {
        a--;
    }

    if (a <= b)
    {
        a = 1;
    }

    if (a >= b)
    {
        a = 2;
    }

    if (a < b)
    {
        a = 3;
    }

    if (a > b)
    {
        a = 4;
    }

    if (a && b)
    {
        a = a & b;
    }

    if (a || b)
    {
        a = a | b;
    }

    a = a ^ b;
    a = ~a;

    return 0;
}