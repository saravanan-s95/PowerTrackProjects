#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "text_editor.h"



Node *create_node(const char *text)
{
    Node *new_node = malloc(sizeof(Node));

    if(new_node == NULL )
    {
        return NULL;
        
    }

    new_node->line = malloc(strlen(text) + 1);

    if(new_node->line == NULL)
    {
        free(new_node);
        return NULL;
    }

    strcpy(new_node->line,text);

    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}

int append_node(Node **head, Node **tail , const char *text)
{
    Node *new_node = create_node(text);
    
    if(new_node == NULL)
    {
        return -1;
    }

    if(*head == NULL)
    {
        *head = new_node;
        *tail = new_node;
    }
    else
    {
        (*tail)->next = new_node;
        new_node->prev = *tail;
        *tail = new_node;
    }

    return 0;
}

void display_document(Node *head)
{
    Node *temp = head;

    while(temp != NULL)
    {
        printf("%s\n",temp->line);
        temp = temp->next;
    }
}
Node *get_node(Node *head,int line_number)
{
    Node *temp = head;
    int count = 1;

    if(line_number < 1)
    {
        return NULL;
    }

    while(temp != NULL && count < line_number)
    {
        temp = temp->next;
        count++;
    }
    return temp;
}

int insert_line(Node **head, Node **tail , int line_number , const char *text)
{
    Node *new_node;
    Node *current;
    Node *previous_node;

    if(line_number < 1)
    {
        return -1;
    }

    new_node = create_node(text);

    if(new_node == NULL)
    {
        return -1;
    }

    if(*head == NULL)
    {
        *head = new_node;
        *tail = new_node;
        return 0;
    }


    if(line_number == 1)
    {
        new_node->next = *head;
        (*head)->prev = new_node;
        *head = new_node;
        return 0;
    }

    current = get_node(*head,line_number - 1);

    if(current == NULL)
    {
        free(new_node->line);
        free(new_node);
        return -1;
    }

    if(current == *tail)
    {
        current->next = new_node;
        new_node->prev = current;
        *tail = new_node;
        return 0;
    }

    previous_node = current;

    current = current->next;

    new_node ->prev = previous_node;
    new_node->next = current;

    previous_node->next = new_node;
    current->prev = new_node;

    return 0;
}

int delete_line(Node **head, Node **tail , int line_number)
{
    Node *current;

    if(line_number < 1)
    {
        return -1;
    }

    current = get_node(*head,line_number);

    if(current == NULL)
        return -1;

    if(current == *head)
    {
        *head = current->next;

        if(*head != NULL)
            (*head)->prev = NULL;
        else
        {
            *tail = NULL;
        }
    }
    else
    {
        if(current->next != NULL)
        {
            current->next->prev = current->prev;
        }

        if(current->prev != NULL)
        {
            current->prev->next = current->next;
        }
    }

    if(current == *tail)
    {
        *tail = current->prev;
    }


    free(current->line);
    free(current);

    return 0;
}