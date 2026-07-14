#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "text_editor.h"
#include "colour.h"

void insert_text()
{
    char text[MAX_LINE_LEN];
    printf(RED "Enter the text : ");
    scanf(" %255[^\n]", text);

    // Creating a new node and storing the text, each node represents single line
    Node *newnode = malloc(sizeof(Node));
    strcpy(newnode->line, text);
    newnode->next = NULL;
    newnode->prev = NULL;

    // If linkedlist is empty then insert at the beginning
    if (head == NULL)
    {
        head = newnode;
    }

    // If node is not empty insert at the end
    else
    {
        Node *temp = head;
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = newnode;
        newnode->prev = temp;
    }

    // Storing the details of the operation into action structure so that it can be used later whenever required
    Action act;
    strcpy(act.operation, "insert");
    strcpy(act.text, text);
    act.line_no = 0;        // Remains unused (0 is just placeholder) because insertion always happens at the end or beginning
    push(&undo_stack, act); // pushes the stack into undo_stack suppose user wants it back
    redo_stack.top = -1;    // Whenever we do any new operation like insert or delete the redo stack gets cleared
}

void delete_text(int line_no)
{
    printf("Enter line number to delete: ");
    scanf("%d", &line_no);
    if (head == NULL)
    {
        printf("Nothing to delete\n");
        return;
    }
    Node *temp = head;
    int count = 1;
    while (temp && count < line_no)
    {
        temp = temp->next;
        count++;
    }
    if (temp == NULL)
    {
        printf("Line %d not found\n", line_no);
        return;
    }
    // Storing the details of the operation into action structure so that it can be used later whenever required
    Action act;
    strcpy(act.operation, "delete");
    strcpy(act.text, temp->line);
    act.line_no = line_no;
    push(&undo_stack, act); // pushes the stack into undo_stack suppose user wants it back
    redo_stack.top = -1;    // Whenever we do any new operation like insert and delete the redo stack gets cleared

    // Handling all edge conditions for deleting a node from linkedlist
    if (temp == head)
    {
        head = temp->next;
    }
    if (temp->prev)
    {
        temp->prev->next = temp->next;
    }
    if (temp->next)
    {
        temp->next->prev = temp->prev;
    }
    free(temp);
    printf(RED "\n✅ Line %d Deleted ✅\n\n", line_no);
}
