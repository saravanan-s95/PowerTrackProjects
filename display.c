#include "text_editor.h"

void display_text(struct Node *head)
{
    struct Node *temp = head;

    if (head == NULL)
    {
        printf("Text editor is empty\n");
        return;
    }

    printf("\n---------- TEXT ----------\n");

    while (temp != NULL)
    {
        printf("%s\n", temp->data);
        temp = temp->next;
    }

    printf("--------------------------\n");
}