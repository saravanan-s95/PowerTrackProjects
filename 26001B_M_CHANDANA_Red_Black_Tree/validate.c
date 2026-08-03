#include "main.h"

int read_integer(data_t *num)
{
    char buffer[100];
    char *endptr;

    // read the input
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return FAILURE;

    // convert string to integer
    *num = (int)strtol(buffer, &endptr, 10);

    // skip the trailing spaces
    while (*endptr == ' ' || *endptr == '\t')
        endptr++;

    // check for invalid characters
    if (*endptr != '\n' && *endptr != '\0')
        return FAILURE;

    return SUCCESS;
}