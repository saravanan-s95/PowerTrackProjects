#include "apc.h"

int validate_CLA(int argc,char *argv[])
{
    if(argc != 4)
        return 0;

if(strchr("+-*/", argv[2][0]) == NULL)
    return 0;

    for(int i=0; argv[1][i]; i++)
    {
        if(!isdigit(argv[1][i]))
            return 0;
    }

    for(int i=0; argv[3][i]; i++)
    {
        if(!isdigit(argv[3][i]))
            return 0;
    }

    return 1;
}