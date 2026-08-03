#include "apc.h"

long long list_to_number(Dlist *head)
{
    long long num = 0;

    while(head)
    {
        num = (num * 10) + head->data;
        head = head->next;
    }

    return num;
}