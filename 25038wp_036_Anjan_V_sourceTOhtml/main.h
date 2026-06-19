#include <stdio.h>
#include <string.h>
#include <ctype.h>


int is_keyword(const char *word);
void print_html_char(FILE *html, char ch);

/* validations */
int has_c_extension(const char *filename);
int has_html_extension(const char *filename);
int validate_args(int argc, char *argv[]);