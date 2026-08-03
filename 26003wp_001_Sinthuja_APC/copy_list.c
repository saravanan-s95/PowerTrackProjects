#include "apc.h"

void copy_list(Dlist *head,
               Dlist **new_head,
               Dlist **new_tail)
{
    while(head)
    {
        insert_last(new_head,
                    new_tail,
                    head->data);

        head = head->next;
    }
}