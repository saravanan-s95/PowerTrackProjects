#include "text_editor.h"

void insert_text(struct Node **head)
{
    char str[SIZE];

    printf("Enter text: ");
    scanf(" %[^\n]", str);

   
    if (search_text(*head, str))
    {
        printf("Text already exists!\n");
        return;
    }

    struct Node *new = malloc(sizeof(struct Node));

    if (new == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    strcpy(new->data, str);
    new->next = NULL;

   
    if (*head == NULL)
    {
        *head = new;
    }
    else
    {
        struct Node *temp = *head;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = new;
    }

    printf("Text inserted successfully\n");
}