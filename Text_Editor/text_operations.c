#include "text_operations.h"

Node *create_node(char *text)
{
    Node *new = malloc(sizeof(Node));

    if(new == NULL)
    {
        return NULL;
    }

    strcpy(new->line,text);

    new->prev = NULL;
    new->next = NULL;

    return new;
}

int get_line_count(Node *head)
{
    int count = 0;

    while(head != NULL)
    {
        count++;

        head = head->next;
    }

    return count;
}

void insert_line(Node **head, Node **tail, char *text)
{
    Node *new = create_node(text);

    if(new == NULL)
    {
        return;
    }

    /* Empty List */
    if(*head == NULL)
    {
        *head = new;
        *tail = new;
    }
    else
    {
        (*tail)->next = new;
        new->prev = *tail;
        *tail = new;
    }

    //printf("Line Inserted Successfully\n");
}


int delete_line(Node **head,
                Node **tail,
                int line_no,
                char *deleted_text)
{
    if(*head == NULL)
    {
        printf("Editor is Empty\n");
        return 0;
    }

    Node *temp = *head;
    int count = 1;

    while(temp != NULL && count < line_no)
    {
        temp = temp->next;
        count++;
    }

    if(temp == NULL)
    {
        printf("Invalid Line Number\n");
        return 0;
    }

    /* Save the text before deleting */
    strcpy(deleted_text, temp->line);

    /* Only one node */
    if(temp == *head && temp == *tail)
    {
        *head = NULL;
        *tail = NULL;
    }

    /* First node */
    else if(temp == *head)
    {
        *head = temp->next;
        (*head)->prev = NULL;
    }

    /* Last node */
    else if(temp == *tail)
    {
        *tail = temp->prev;
        (*tail)->next = NULL;
    }

    /* Middle node */
    else
    {
        temp->prev->next = temp->next;
        temp->next->prev = temp->prev;
    }

    free(temp);

    printf("Line Deleted Successfully\n");

    return 1;
}


char clipboard[MAX_LINE_LENGTH] = "";


void copy_text(Node *head, int line_no)
{
    Node *temp = head;
    int count = 1;

    while(temp != NULL && count < line_no)
    {
        temp = temp->next;
        count++;
    }

    if(temp == NULL)
    {
        printf("Invalid Line Number\n");
        return;
    }

    strcpy(clipboard, temp->line);

    printf("Line Copied Successfully\n");
}



void paste_text(Node **head, Node **tail)
{
    if(strlen(clipboard) == 0)
    {
        printf("Clipboard is Empty\n");
        return;
    }

    insert_line(head, tail, clipboard);
}


void cut_text(Node **head,Node **tail,int line_no)
{
    if(delete_line(head, tail, line_no, clipboard))
    {
        printf("Line Cut Successfully\n");
    }
}