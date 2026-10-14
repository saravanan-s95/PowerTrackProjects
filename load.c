#include "text_editor.h"

void load_text(struct Node **head)
{
    FILE *fp;
    char str[SIZE];

    fp = fopen("text.txt", "r");

    if (fp == NULL)
    {
        printf("File not found\n");
        return;
    }

    while (fscanf(fp, " %[^\n]", str) != EOF)
    {
        /* Avoid duplicate */
        if (search_text(*head, str))
        {
            continue;
        }

        struct Node *new = malloc(sizeof(struct Node));

        if (new == NULL)
        {
            printf("Memory allocation failed\n");
            fclose(fp);
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
    }

    fclose(fp);

    printf("Text loaded successfully\n");
}