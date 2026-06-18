#include<stdio.h>
int main()
{
    int a = 10;
    float b = 3.14;
    char c = 'x';
    char str[] = "Hello, World!";
    if (a > 5) {
        printf("a is greater than 5\n");
    }
    for (int i = 0; i < a; i++) {
        printf("%d ", i);
    }
    printf("\n");
    return 0;
}