#include "main.h"

/*
    return :
        1 -> valid .c file
        0 -> invalid file
*/
int has_c_extension(const char *filename)
{
    // last occurrence of '.' in the filename
    const char *ext = strrchr(filename, '.');
    // whether extension exists and is ".c
    if(ext != NULL && strcmp(ext, ".c") == 0)
    {
        return 1;
    }

    return 0;
}

/*
    return :
        1 -> valid .html ot .hmt file
        0 -> invalid file
*/
int has_html_extension(const char *filename)
{
    // last occurrence of '.' in the filename
    const char *ext = strrchr(filename, '.');
    // whether extension exists and is ".html" or ".hmt"
    if(ext != NULL)
    {
        if(strcmp(ext, ".html") == 0 ||
           strcmp(ext, ".htm") == 0)
        {
            return 1;
        }
    }

    return 0;
}

int validate_args(int argc, char *argv[])
{
    // cla argument : /a.out sample.c output.html
    if(argc != 3)
    {
        printf("ERROR: Invalid number of arguments\n");
        printf("Usage: %s <input.c> <output.html>\n",
               argv[0]);
        return 0;
    }
    // validate source file extension
    if(!has_c_extension(argv[1]))
    {
        printf("ERROR: %s is not a C source file\n",
               argv[1]);
        return 0;
    }
    // validate output file extension
    if(!has_html_extension(argv[2]))
    {
        printf("ERROR: Output file must have .html or .htm extension\n");
        return 0;
    }
    // All validations passed
    return 1;
}