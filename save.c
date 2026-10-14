#include "text_editor.h"

void save_text(struct Node *head)
{
    FILE *fp;
    struct Node *temp = head;

    fp = fopen("text.txt", "w");

    if (fp == NULL)
    {
        printf("File cannot be opened\n");
        return;
    }

    while (temp != NULL)
    {
        fprintf(fp, "%s\n", temp->data);
        temp = temp->next;
    }

    fclose(fp);

    printf("Text saved successfully\n");
}