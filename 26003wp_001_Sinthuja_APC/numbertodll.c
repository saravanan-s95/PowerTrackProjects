#include "apc.h"

void number_to_list(long long num,
                    Dlist **head,
                    Dlist **tail)
{
    if(num == 0)
    {
        insert_last(head, tail, 0);
        return;
    }

    char str[100];
    sprintf(str,"%lld",num);

    for(int i=0; str[i]; i++)
    {
        insert_last(head,
                    tail,
                    str[i]-'0');
    }
}