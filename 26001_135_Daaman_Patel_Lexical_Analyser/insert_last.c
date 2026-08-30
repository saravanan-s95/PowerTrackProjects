#include "lexical.h"
#include "types.h"

Status insert_last(File_List **head, File_List *new_node)
{
    // If list is empty
    if (*head == NULL)
    {
        *head = new_node;
        return e_success;
    }

    // If list is non-empty
    File_List *temp = *head;
    while (temp->link != NULL)
        temp = temp->link;

    temp->link = new_node;
    return e_success;
}