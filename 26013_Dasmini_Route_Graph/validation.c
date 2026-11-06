#include <stdio.h>
#include "validation.h"
#include "utils.h"

int check_distance(float distance)
{
    if(distance <= 0)
        return NOT_VALID;
    return 0;
}

int check_city(char city[])
{
    for(int i = 0; city[i] != '\0'; i++)
    {
        if(!((city[i] >= 'A' && city[i] <= 'Z') || (city[i] >= 'a' && city[i] <= 'z')))
        {
            return NOT_VALID;
        }
    }
    return 0;
}
