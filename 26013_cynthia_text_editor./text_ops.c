
#include <stdlib.h>
#include <string.h>
#include "text_ops.h"

char *clipboard = NULL;

int insert_char(Node *cursor, int position, char ch)
{
    int length;
    char *new_line;

    if (cursor == NULL || cursor->line == NULL)
    {
        return -1;
    }

    length = strlen(cursor->line);

    if (position < 0 || position > length)
    {
        return -1;
    }

    new_line = realloc(cursor->line, length + 2);

    if (new_line == NULL)
    {
        return -1;
    }

    cursor->line = new_line;

    for (int i = length; i >= position; i--)
    {
        cursor->line[i + 1] = cursor->line[i];
    }

    cursor->line[position] = ch;

    return 0;
}


int delete_char(Node *cursor, int position)
{
    int length;

    if (cursor == NULL || cursor->line == NULL)
    {
        return -1;
    }

    length = strlen(cursor->line);

    if (position < 0 || position >= length)
    {
        return -1;
    }

    for (int i = position; i < length; i++)
    {
        cursor->line[i] = cursor->line[i + 1];
    }

    char *temp = realloc(cursor->line, length);

    if (temp != NULL)
    {
        cursor->line = temp;
    }
    return 0;
}


int copy_line(Node *cursor)
{
    char *new_clipboard;
    size_t length;

    if (cursor == NULL || cursor->line == NULL)
    {
        return -1;
    }

    length = strlen(cursor->line);

    new_clipboard = malloc(length + 1);

    if (new_clipboard == NULL)
    {
        return -1;
    }

    strcpy(new_clipboard, cursor->line);

    free(clipboard);
    clipboard = new_clipboard;

    return 0;
}

int cut_line(Node **head, Node **tail, Node *cursor)
{
    Node *temp;
    int line_number = 1;

    if (head == NULL || tail == NULL || cursor == NULL)
    {
        return -1;
    }

    if (copy_line(cursor) != 0)
    {
        return -1;
    }

    temp = *head;

    while (temp != NULL && temp != cursor)
    {
        temp = temp->next;
        line_number++;
    }

    if (temp == NULL)
    {
        return -1;
    }

    return delete_line(head, tail, line_number);
}

int paste_line(Node **head, Node **tail, int line_number)
{
    if (head == NULL || tail == NULL || clipboard == NULL)
    {
        return -1;
    }

    return insert_line(head, tail, line_number, clipboard);
}
