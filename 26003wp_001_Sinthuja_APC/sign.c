#include "apc.h"

int get_sign(char *str)
{
    if(str[0] == '-')
        return -1;

    return 1;
}

char *skip_sign(char *str)
{
    if(str[0] == '+' || str[0] == '-')
        return str + 1;

    return str;
}