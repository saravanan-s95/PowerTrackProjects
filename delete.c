#include "text_editor.h"

void delete_text(struct Node **head)
{
    char str[SIZE];

    printf("Enter text to delete: ");
    scanf(" %[^\n]", str);

    struct Node *temp = *head;
    struct Node *prev = NULL;

    while (temp != NULL)
    {
        if (strcmp(temp->data, str) == 0)
        {
            /* Delete first node */
            if (prev == NULL)
            {
                *head = temp->next;
            }
            else
            {
                prev->next = temp->next;
            }

            free(temp);

            printf("Text deleted successfully\n");
            return;
        }

        prev = temp;
        temp = temp->next;
    }

    printf("Text not found\n");
}