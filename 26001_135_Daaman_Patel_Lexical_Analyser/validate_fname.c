#include "types.h"
#include "lexical.h"

Status validate_fname(char f_name[])
{
    // Now check for dot
    char *ptr = strchr(f_name, '.');

    // If dot is not present then return error
    if (ptr == NULL)
    {
        printf("ERROR: Dot is not present in file %s\n", f_name);
        return e_failure;
    }

    // Now check for extension
    if (strcmp(ptr, ".c"))
    {
        printf("ERROR: Invalid extension for file %s\n", f_name);
        return e_failure;
    }

    // Checking if file is present or not
    FILE *fp = fopen(f_name, "r");

    if (fp == NULL)
    {
        printf("ERROR: %s file not present\n", f_name);
        return e_failure;
    }

    return e_success;
}