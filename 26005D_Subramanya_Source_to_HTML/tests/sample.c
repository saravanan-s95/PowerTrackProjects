#include <stdio.h>
#include "dir/example.h"
#include MYHEADER
#define MAX 100

/* multi-line
 * comment with <angle> & ampersand 
 * Hello world */

// single-line comment with <html> & stuff

int main(void)
{
	char c = 'a';
	char nl = '\n';
	char *s = "a < b & c, \"quoted\"";
	float f = 3.14;
	int arr[3] = {1, 2, 3};
	int a = 5, b = 10;

	if (a <= b && b > 3 || a != b)
		printf("hi %d\n", arr[a - 4]);

	for (a = 0; a < MAX; a++)
		b += a;

	while (b >= 0)
		b -= 2;

	return sizeof(int) / 2;
}