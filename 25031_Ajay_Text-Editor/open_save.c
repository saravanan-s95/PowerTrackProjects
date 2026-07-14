#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "text_editor.h"
#include "colour.h"

// open_file converts text file data to linkedlist node(s) data
void open_file()
{
    // Opens file in a read mode
    FILE *fp = fopen("text.txt", "r");

    // Give error if unable to find saved file
    if (fp == NULL)
    {
        printf("\nNo saved file found\n\n");
        return;
    }

    char buffer[MAX_LINE_LEN]; // Char array of size 100bytes to store the data of each line
    int lines = 0;             // Line count to display user how many lines are loaded from file

    // fscanf reads whole line from file(fp) and stores it in buffer, this reading goes on till end of file is reached
    while (fscanf(fp, " %[^\n]", buffer) != EOF)
    {
        // For each line new node is created
        Node *new = malloc(sizeof(Node));
        // Copying the buffer content into new->line
        strcpy(new->line, buffer);
        new->next = NULL;
        new->prev = NULL;

        // Logic to insert at the end
        if (head == NULL)
        {
            head = new;
        }
        else
        {
            Node *temp = head;
            while (temp->next != NULL)
                temp = temp->next;

            temp->next = new;
            new->prev = temp;
        }

        // Increment line counter to keep track of lines
        lines++;
    }
    // Close the file
    fclose(fp);
    // If line count is zero file is empty
    if (lines == 0)
    {
        printf("\nFile is empty\n\n");
    }
    else
    {
        printf(GREEN "\nLoaded %d lines from file\n\n", lines);
    }
}

// save() converts linkedlist node data to file lines
void save()
{
    // Opens file in a write mode
    FILE *fp = fopen("text.txt", "w");
    if (fp == NULL)
    {
        printf(RED "\nError opening file for saving\n\n");
        return;
    }

    // temp points to head
    Node *temp = head;

    // If temp is null nothing to save (text is empty)
    if (temp == NULL)
    {
        printf(RED "\nNothing to save (text is empty)\n\n");
        fclose(fp);
        return;
    }

    // Read the data that is present till the end of list
    while (temp != NULL)
    {
        // writes the stored data in node (temp->line) to file(fp)
        fprintf(fp, "%s\n", temp->line);
        temp = temp->next;
    }
    // close the file to avoid leaks
    fclose(fp);
    // print the success message to user
    printf(RED "\nText saved successfully to text.txt\n\n");
}
