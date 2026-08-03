#include "apc.h"

void remove_leading_zeros(Dlist **head,
                          Dlist **tail)
{
    while((*head) &&
          (*head)->data == 0 &&
          (*head)->next)
    {
        Dlist *temp = *head;

        *head = (*head)->next;
        (*head)->prev = NULL;

        free(temp);
    }
}