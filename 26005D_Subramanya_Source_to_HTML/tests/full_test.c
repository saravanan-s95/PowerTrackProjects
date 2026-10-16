#include <stdio.h>
#include <stdlib.h>

/* =========================================
   SOURCE-TO-HTML LEXER TEST FILE
   Tests: directives, keywords, literals,
   operators, symbols, comments and whitespace
   ========================================= */

#define MAX_SIZE 10
#define PI 3.14159

// Type keywords and identifiers
int count = 10;
float temperature = 25.5f;
char letter = 'A';
char newline = '\n';

const char *message = "Hello, World!";

// Header-like text inside a string
char *html_text = "<stdio.h> & <stdlib.h>";

// Array declaration and initialization
int numbers[MAX_SIZE] = {1, 2, 3, 4, 5};

// Arithmetic and comparison operators
int add(int a, int b)
{
    return a + b;
}

/* Multiline comment:
   Check whether <, >, &, *, / are preserved.
   int fake_code = 100;
*/

int main(void)
{
    int a = 10;
    int b = 20;
    int result = add(a, b);

    // Integer and floating-point constants
    int hex_value = 0xFF;
    int octal_value = 077;
    float decimal = 123.456f;
    double scientific = 1.2e-3;

    // Conditional and logical operators
    if (a < b && b >= 10)
    {
        printf("Result = %d\n", result);
    }
    else if (a == b || a != 0)
    {
        printf("Condition matched\n");
    }
    else
    {
        printf("No match\n");
    }

    // Loops and increment/decrement operators
    for (int i = 0; i < 5; i++)
    {
        printf("numbers[%d] = %d\n", i, numbers[i]);
    }

    while (count > 0)
    {
        count--;
    }

    // Bitwise and compound assignment operators
    unsigned int flags = 0x0F;
    flags |= 0x10;
    flags &= 0x1F;
    flags ^= 0x01;
    flags <<= 1;
    flags >>= 1;

    // Pointer and address operators
    int *ptr = &result;
    *ptr += 5;

    // Escape sequences and special characters
    printf("Tab:\tNew line:\n");
    printf("Quotes: \"Hello\"\n");
    printf("Backslash: \\\n");
    printf("Characters: %c %c\n", 'X', '\t');

    // Ternary operator
    const char *status = (result > 0) ? "positive" : "non-positive";
    printf("Status: %s\n", status);

    // Structure declaration and member access
    struct Sensor
    {
        int id;
        float value;
    };

    struct Sensor sensor = {1, 36.5f};
    printf("Sensor %d: %.2f\n", sensor.id, sensor.value);

    return 0;
}
