#include "apc.h"

int insert_last(Dlist **head,Dlist **tail,int data)
{
    Dlist *new = malloc(sizeof(Dlist));

    if(new == NULL)
        return 0;

    new->data = data;
    new->next = NULL;
    new->prev = NULL;

    if(*head == NULL)
    {
        *head = *tail = new;
        return 1;
    }

    (*tail)->next = new;
    new->prev = *tail;
    *tail = new;

    return 1;
}

int insert_first(Dlist **head,Dlist **tail,int data)
{
    Dlist *new = malloc(sizeof(Dlist));

    if(new == NULL)
        return 0;

    new->data = data;
    new->prev = NULL;
    new->next = *head;

    if(*head == NULL)
    {
        *head = *tail = new;
        return 1;
    }

    (*head)->prev = new;
    *head = new;

    return 1;
}

void create_list(Dlist **head,Dlist **tail,char *str)
{
    int i = 0;

    while(str[i])
    {
        insert_last(head,
                    tail,
                    str[i] - '0');
        i++;
    }
}

void print_list(Dlist *head)
{
    while(head)
    {
        printf("%d",head->data);
        head = head->next;
    }
}