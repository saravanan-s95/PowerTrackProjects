#include "cursor_point.h"

void init_cursor(Cursor *cursor, Node *head)
{
    cursor->current = head;
    cursor->row = 1;
    cursor->col = 1;
}

void move_up(Cursor *cursor)
{
    if(cursor->current == NULL)
    {
        printf("Editor is Empty\n");
        return;
    }

    if(cursor->current->prev == NULL)
    {
        printf("Already at First Line\n");
        return;
    }

    cursor->current = cursor->current->prev;
    cursor->row--;
}

void move_down(Cursor *cursor)
{
    if(cursor->current == NULL)
    {
        printf("Editor is Empty\n");
        return;
    }

    if(cursor->current->next == NULL)
    {
        printf("Already at Last Line\n");
        return;
    }

    cursor->current = cursor->current->next;
    cursor->row++;
}

void show_cursor(Cursor *cursor)
{
    if(cursor->current == NULL)
    {
        printf("Cursor is not pointing to any line\n");
        return;
    }

    printf("\nCurrent Line : %d\n", cursor->row);
    printf("Text       : %s\n", cursor->current->line);
}