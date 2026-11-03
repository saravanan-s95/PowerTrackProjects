#include<stdio.h>
#include "cursor_navigation.h"

int move_cursor_next(Node **cursor)
{
    if (cursor == NULL || *cursor == NULL)
    {
        return -1;
    }

    if ((*cursor)->next == NULL)
    {
        return -1;
    }

    *cursor = (*cursor)->next;

    return 0;
}

int move_cursor_previous(Node **cursor)
{
    if (cursor == NULL || *cursor == NULL)
    {
        return -1;
    }

    if ((*cursor)->prev == NULL)
    {
        return -1;
    }

    *cursor = (*cursor)->prev;

    return 0;
}
