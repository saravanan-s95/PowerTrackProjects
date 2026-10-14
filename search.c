#include "text_editor.h"

int search_text(struct Node *head, char *str)
{
    struct Node *temp = head;

    while (temp != NULL)
    {
        if (strcmp(temp->data, str) == 0)
        {
            return 1;
        }

        temp = temp->next;
    }

    return 0;
}