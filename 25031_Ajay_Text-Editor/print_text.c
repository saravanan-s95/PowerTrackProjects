#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "text_editor.h"
#include "colour.h"

extern Node *head;
void print_text()
{
    if (head == NULL)
    {
        printf(YELLOW "\n\nText is Empty(use Open to load file)\n\n");
        return;
    }
    Node *temp = head;
    int line_no = 1;
    printf(BLUE "\n-----------Text---------\n");
    while (temp != NULL)
    {
        printf("%d : %s\n", line_no, temp->line);
        temp = temp->next;
        line_no++;
    }
    printf("------------------------\n\n");
}
