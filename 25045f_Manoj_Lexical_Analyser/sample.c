#include <stdio.h>

int main()
{
   /* int is_integer_constant(char *token);
int is_float_constant(char *token);
int is_character_constant(char *token);
int is_string_literal(char *token);
int is_identifier(char *token); */
    int a = 10;
    float b = 12.3;
    int hex = 0x1A;
    char str[] = "hello";
    a++;
    a -= 3;
    a += 3;

    char ch = 'a';

    if(a >= 5)
    {
        printf("Hello\n");
    }

    return 0;
}