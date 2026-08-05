#ifndef APC_H
#define APC_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define SUCCESS -1
#define FAILURE -2
typedef struct node{
    int digit;
    struct node* prev;
    struct node* next;
}Dlist;

typedef struct stack{
    Dlist* curr_dividend_head;
    Dlist* curr_dividend_tail;
    Dlist* bit_head;
    Dlist* bit_tail;
    struct stack* next;
}stacknode;

int validatenum(char* str);
int insertatfirst(Dlist**head,Dlist**tail,int data);
int insertatlast(Dlist**head,Dlist**tail,int data);
void print_list(Dlist* head);
void createlist(Dlist** head,Dlist**tail,char* str);
int compare(Dlist* head1,Dlist* head2);

Dlist* addition(char* str1,char*str2);
Dlist* add(Dlist* head1,Dlist*tail1,Dlist*head2,Dlist* tail2,Dlist**head,Dlist**tail);

Dlist* subtraction(char* str1,char* str2);
Dlist* sub(Dlist* head1,Dlist* tail1,Dlist* head2,Dlist*tail2,Dlist**head,Dlist**tail);

Dlist* multiplication(char* str1,char* str2);
Dlist* mul(Dlist* head1,Dlist* tail1,Dlist* head2,Dlist* tail2);

Dlist* division(char* str1,char*str2);
void divi(Dlist*head1,Dlist* tail1,Dlist* head2,Dlist* tail2,Dlist** quot,Dlist** rem);

Dlist* modulo(char* str1, char* str2);

#endif

/* Author: Swati Pathak*/
/*Project: APC-Arbitary Precision Calculator*/
/*Description: Normally when we do any arithmetic operation there is limit for the number for 32 bits for int or 64 bits for long.
                In order to overcome this limitation we have made this project where the arithmetic operation doesnot get limited 
                due to size of int or long int. we can calculate any big number operation with this method.
                In this project I have used Linked list concept where each number is first converted to a list of node where each node 
                holds digit in the base of 10000 except the first node which can or canot be a base of 10000.
                In this project the I have implemented addition,multiplication,subtraction,division and modulo operation*/