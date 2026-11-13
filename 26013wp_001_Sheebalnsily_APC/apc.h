#ifndef APC_H
#define APC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SUCCESS 1
#define FAILURE 0

/* Doubly linked list node */
typedef struct node
{
    int data;
    struct node *prev;
    struct node *next;
} Dlist;


/*----------------------------------------------------------
                    Linked List Operations
----------------------------------------------------------*/

int insert_at_first(Dlist **head,
                    Dlist **tail,
                    int data);

int insert_at_last(Dlist **head,
                   Dlist **tail,
                   int data);

int delete_at_first(Dlist **head,
                    Dlist **tail);

int delete_at_last(Dlist **head,
                   Dlist **tail);

void print_list(Dlist *head);


/*----------------------------------------------------------
                    Arithmetic Operations
----------------------------------------------------------*/

/* Addition */
int addition(Dlist *head1,
             Dlist *tail1,
             Dlist *head2,
             Dlist *tail2,
             Dlist **headR,
             Dlist **tailR);


/* Subtraction */
int subtraction(Dlist *head1,
                Dlist *tail1,
                Dlist *head2,
                Dlist *tail2,
                Dlist **headR,
                Dlist **tailR);


/* Multiplication */
int multiplication(Dlist *head1,
                   Dlist *tail1,
                   Dlist *head2,
                   Dlist *tail2,
                   Dlist **headR,
                   Dlist **tailR);


/* Integer Division */
int division(Dlist *head1,
             Dlist *tail1,
             Dlist *head2,
             Dlist *tail2,
             Dlist **headR,
             Dlist **tailR);


/* Decimal Division */
int division_decimal(Dlist *head1,
                     Dlist *tail1,
                     Dlist *head2,
                     Dlist *tail2,
                     int decimal_places);


#endif