#include <stdlib.h>
#include <stdio.h>
#include "apc.h"


int delete_list(Dlist **head, Dlist **tail)
{
    if(*head == NULL)
    {
        *tail = NULL;
        printf("List is empty");
        return 0;
    }

    Dlist *temp1;
    
    while(*head != NULL)
    {
        temp1 = *head;
        (*head) = (*head)->next;
        free(temp1);
    }

    *tail = NULL;
    return 1;
}

void remove_leading_zeros(Dlist **head, Dlist **tail)
{
    while (*head != NULL &&
           (*head)->data == 0 &&
           (*head)->next != NULL)
    {
        Dlist *temp = *head;

        *head = (*head)->next;
        (*head)->prev = NULL;

        free(temp);
    }

    if (*head == NULL)
    {
        *tail = NULL;
    }
}

int list_compare(Dlist *head1, Dlist *tail1, Dlist *head2, Dlist *tail2)
{
    Dlist *list1 = head1;
    Dlist *list2 = head2;
    int count1 = 0;
    int count2 = 0;
    while (list1 != NULL)
    {
        count1++;
        list1 = list1->next;
    }

    while (list2 != NULL)
    {
        count2++;
        list2 = list2->next;
    }

    if (count1 > count2)
    {
        return 1;
    }
    else if (count1 < count2)
    {
        return 0;
    }
    else if (count1 == count2)
    {
        Dlist *temp1 = head1;
        Dlist *temp2 = head2;
        while (temp1 != NULL && temp2 != NULL)
        {
            if (temp1->data > temp2->data)
            {
                return 1;
            }
            else if (temp1->data < temp2->data)
            {
                return 0;
            }
            else
            {
                temp1 = temp1->next;
                temp2 = temp2->next;
            }
        }
    }
    return 1;
}

int insert_first(Dlist **head, Dlist **tail, int data)
{
    Dlist *newNode = malloc(sizeof(Dlist));
    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    if (*head == NULL)
    {
        *head = newNode;
        *tail = newNode;
        return SUCCESS;
    }

    newNode->next = *head;
    (*head)->prev = newNode;
    *head = newNode;
    return SUCCESS;
}

void print_Dlist(Dlist **head, Dlist **tail)
{
    // Delete leading zeroes
    while (*head != NULL && (*head)->data == 0)
    {
        Dlist *temp = *head;

        *head = (*head)->next;

        if (*head != NULL)
        {
            (*head)->prev = NULL;
        }
        else
        {
            *tail = NULL;
        }

        free(temp);
    }

    // Check empty list
    if (*head == NULL)
    {
        printf("0\n");
        return;
    }

    // Print list
    Dlist *temp = *head;

    while (temp != NULL)
    {
        printf("%d", temp->data);
        temp = temp->next;
    }

    printf("\n");
}

int insert_last(Dlist **head1, Dlist **tail1, int data)
{
    Dlist *newNode = malloc(sizeof(Dlist));
    if (newNode == NULL)
    {
        return FAILURE;
    }

    newNode->data = data;
    newNode->next = NULL;
    newNode->prev = NULL;

    if (*head1 == NULL)
    {
        *head1 = newNode;
        *tail1 = newNode;
        return SUCCESS;
    }

    (*tail1)->next = newNode;
    newNode->prev = *tail1;
    *tail1 = newNode;
    return SUCCESS;
}

void digit_to_list(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, char *argv[])
{
    char *list1 = argv[1];
    char *list2 = argv[3];
    int i = 0;
    while (list1[i] != '\0')
    {
        int result = list1[i] - '0';
        insert_last(head1, tail1, result);
        i++;
    }

    int j = 0;
    while (list2[j] != '\0')
    {
        int result = list2[j] - '0';
        insert_last(head2, tail2, result);
        j++;
    }
    // return SUCCESS;
}