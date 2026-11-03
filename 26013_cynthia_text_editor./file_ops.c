
#include <stdio.h>
#include "file_ops.h"
#include <stdlib.h>
#include <string.h>

int save_file(Node *head, const char *filename)
{
    FILE *fp;
    Node *temp = head;

    if (filename == NULL)
        return -1;

    fp = fopen(filename, "w");

    if (fp == NULL)
        return -1;

    while (temp != NULL)
    {
        if (fprintf(fp, "%s\n", temp->line) < 0)
        {
            fclose(fp);
            return -1;
        }

        temp = temp->next;
    }

    if (fclose(fp) != 0)
        return -1;

    return 0;
}



int open_file(Node **head, Node **tail, const char *filename)
{
    FILE *fp;
    char buffer[1024];
    Node *new_head = NULL;
    Node *new_tail = NULL;

    if (head == NULL || tail == NULL || filename == NULL)
        return -1;

    fp = fopen(filename, "r");

    if (fp == NULL)
        return -1;

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        size_t len = strlen(buffer);

        if (len > 0 && buffer[len - 1] == '\n')
            buffer[len - 1] = '\0';

        if (append_node(&new_head, &new_tail, buffer) != 0)
        {
            fclose(fp);

            while (new_head != NULL)
            {
                Node *temp = new_head;
                new_head = new_head->next;
                free(temp->line);
                free(temp);
            }

            return -1;
        }
    }

    if (ferror(fp))
    {
        fclose(fp);

        while (new_head != NULL)
        {
            Node *temp = new_head;
            new_head = new_head->next;
            free(temp->line);
            free(temp);
        }

        return -1;
    }

    fclose(fp);

    /* Replace the existing document only after reading succeeds. */
    while (*head != NULL)
    {
        Node *temp = *head;
        *head = (*head)->next;
        free(temp->line);
        free(temp);
    }

    *head = new_head;
    *tail = new_tail;

    return 0;
}
