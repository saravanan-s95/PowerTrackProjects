#ifndef APC_H
#define APC_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

typedef struct node
{
    int data;
    struct node *prev;
    struct node *next;
}Dlist;

/* validation */
int validate_CLA(int argc,char *argv[]);

/* list functions */
int insert_first(Dlist **head,Dlist **tail,int data);
int insert_last(Dlist **head,Dlist **tail,int data);
void create_list(Dlist **head,Dlist **tail,char *str);
void print_list(Dlist *head);
int get_sign(char *str);
char *skip_sign(char *str);
/* addition */
int addition(Dlist *head1,Dlist *tail1,
             Dlist *head2,Dlist *tail2,
             Dlist **res_head,Dlist **res_tail);

/* subtraction */
int subtraction(Dlist *head1,Dlist *tail1,
                Dlist *head2,Dlist *tail2,
                Dlist **res_head,Dlist **res_tail);

int compare_lists(Dlist *head1,Dlist *head2);

void remove_leading_zeros(Dlist **head,
                          Dlist **tail);

int multiplication(Dlist *head1, Dlist *tail1,
                   Dlist *head2, Dlist *tail2,
                   Dlist **res_head,
                   Dlist **res_tail);

void append_zeros(Dlist **head,
                  Dlist **tail,
                  int count);

void copy_list(Dlist *head,
               Dlist **new_head,
               Dlist **new_tail);

int division(Dlist *head1, Dlist *tail1,
             Dlist *head2, Dlist *tail2,
             Dlist **res_head,
             Dlist **res_tail);

long long list_to_number(Dlist *head);

void number_to_list(long long num,
                    Dlist **head,
                    Dlist **tail);

#endif
