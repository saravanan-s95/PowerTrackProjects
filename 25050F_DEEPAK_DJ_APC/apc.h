#ifndef APC_H
#define APC_H
#define SUCCESS 0
#define FAILURE -1

typedef int data_t;
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct node
{
	struct node *prev;
	data_t data;
	struct node *next;
}Dlist;
void print_list(Dlist *head);
void insert_at_end(Dlist **head,Dlist **tail,int data);
void insert_at_begining(Dlist **head,Dlist **tail,int data);

void addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headr,Dlist **tailr);
int list_compare(Dlist **head1,Dlist **head2);

void subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headr,Dlist **tailr,int flag);
void division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headr,Dlist **tailr,int *count);
void delete_list(Dlist **head,Dlist **tail);
int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headr, Dlist **tailr);
#endif