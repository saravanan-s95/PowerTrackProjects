#include "apc.h"

void append_zeros(Dlist **head,
                  Dlist **tail,
                  int count)
{
    while(count--)
    {
        insert_last(head,
                    tail,
                    0);
    }
}