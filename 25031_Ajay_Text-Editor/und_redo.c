#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "text_editor.h"
#include "colour.h"

void undo()
{
    if (is_empty(&undo_stack))
    {
        printf(YELLOW "\nNothing to undo\n\n");
        return;
    }
    // pop & store topmost action structure in undo+_stack
    Action act = pop(&undo_stack);

    // In undo insert means deleting that node to make it like before,since insertion happens at the end deletion also should happen at the end to remove same node
    if (strcmp(act.operation, "insert") == 0)
    {
        if (head == NULL)
        {
            return;
        }
        Node *temp = head;
        while (temp->next)
        {
            temp = temp->next;
        }
        if (temp->prev)
        {
            temp->prev->next = NULL;
        }
        else
        {
            head = NULL;
        }
        free(temp);
    }
    else if (strcmp(act.operation, "delete") == 0)
    {
        // Create new node to insert the deleted node(text)
        Node *newnode = malloc(sizeof(Node));
        strcpy(newnode->line, act.text); // Copying the text that was deleted to restore it again
        newnode->next = NULL;
        newnode->prev = NULL;

        // Insertion logic
        if (head == NULL)
            head = newnode;
        else
        {
            Node *temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newnode;
            newnode->prev = temp;
        }
    }
    // Push the act that is undo to the redo stack so it is available if incase we want to redo
    push(&redo_stack, act);
    printf(GREEN "\n✅ Undo successful ✅\n\n");
}

void redo()
{
    if (is_empty(&redo_stack))
    {
        printf(YELLOW "\nNothing to redo\n\n");
        return;
    }
    // pop & store topmost action structure in redo_Stack
    Action act = pop(&redo_stack);
    // In operation is insert then we are going to insert the node
    if (strcmp(act.operation, "insert") == 0)
    {
        Node *newnode = malloc(sizeof(Node));
        strcpy(newnode->line, act.text); // Copy the text that needs to be inserted
        newnode->next = NULL;
        newnode->prev = NULL;

        if (head == NULL)
            head = newnode;
        else
        {
            Node *temp = head;
            while (temp->next)
                temp = temp->next;
            temp->next = newnode;
            newnode->prev = temp;
        }
        printf(GREEN "\nRedo: insert line restored\n\n");
    }
    // Logic to delete the node
    else if (strcmp(act.operation, "delete") == 0)
    {
        if (head == NULL)
            return;

        Node *temp = head;
        while (temp->next)
            temp = temp->next;

        if (temp->prev)
            temp->prev->next = NULL;
        else
            head = NULL;

        free(temp);
        printf(GREEN "\nRedo: deleted line removed again\n\n");
    }
    // Pushing the redo act to undo stack incase user wants to undo
    push(&undo_stack, act);
}
