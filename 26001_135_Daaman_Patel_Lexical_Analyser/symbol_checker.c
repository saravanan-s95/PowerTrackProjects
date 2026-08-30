#include "types.h"
#include "lexical.h"

Status is_symbol(char ch)
{
    char symbols[] = {')', '(', '{', '}', '[', ']', ';', ','};

    for (int i = 0; i < 8; i++)
    {
        if (symbols[i] == ch)
            return e_success;
    }

    return e_failure;
}