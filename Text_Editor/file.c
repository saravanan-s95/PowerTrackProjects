#include "file.h"
#include "text_operations.h"

void open_file(Node **head, Node **tail, char *filename)
{
    FILE *fp = fopen(filename, "r");

    if(fp == NULL)
    {
        printf("Unable to Open File\n");
        return;
    }

    char line[MAX_LINE_LENGTH];

    while(fgets(line, MAX_LINE_LENGTH, fp))
    {
        line[strcspn(line, "\n")] = '\0';

        insert_line(head, tail, line);
    }

    fclose(fp);

    printf("File Opened Successfully\n");
}

void save_file(Node *head, char *filename)
{
    FILE *fp = fopen(filename, "w");

    if(fp == NULL)
    {
        printf("Unable to Save File\n");
        return;
    }

    Node *temp = head;

    while(temp != NULL)
    {
        fprintf(fp, "%s\n", temp->line);

        temp = temp->next;
    }

    fclose(fp);

    printf("File Saved Successfully\n");
}

void close_file(void)
{
    printf("File Closed Successfully\n");
}