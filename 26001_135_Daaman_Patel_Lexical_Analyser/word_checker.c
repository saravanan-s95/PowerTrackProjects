#include "types.h"
#include "lexical.h"

Status isword(char word[])
{
    int i = 0;
    while (word[i])
    {
        if (isalpha(word[i]) == 0 && word[i] != '_')
        {
            return e_failure;
        }

        i++;
    }

    return e_success;
}