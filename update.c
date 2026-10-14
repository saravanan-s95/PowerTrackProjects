#include "text_editor.h"

void update_text(struct Node *head)
{
    char old_text[SIZE];
    char new_text[SIZE];

    printf("Enter text to update: ");
    scanf(" %[^\n]", old_text);

    struct Node *temp = head;

    while (temp != NULL)
    {
        if (strcmp(temp->data, old_text) == 0)
        {
            printf("Enter new text: ");
            scanf(" %[^\n]", new_text);

            /* Don't allow duplicate */
            if (search_text(head, new_text))
            {
                printf("New text already exists!\n");
                return;
            }

            strcpy(temp->data, new_text);

            printf("Text updated successfully\n");
            return;
        }

        temp = temp->next;
    }

    printf("Text not found\n");
}