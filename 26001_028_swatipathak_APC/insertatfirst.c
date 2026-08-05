#include "header.h"
int insertatfirst(Dlist**head,Dlist**tail,int data){
    Dlist* new=malloc(sizeof(Dlist));
    if(new==NULL)
        return FAILURE;
    new->prev=NULL;
    new->digit=data;
    new->next=*head;
    if(*head==NULL){
        *head=*tail=new;
        return SUCCESS;
    }
    else{
        (*head)->prev=new;
        *head=new;
        return SUCCESS;
    }
}

/* Author: Swati Pathak*/
/*Project: APC-Arbitary Precision Calculator*/
/*Description: Normally when we do any arithmetic operation there is limit for the number for 32 bits for int or 64 bits for long.
                In order to overcome this limitation we have made this project where the arithmetic operation doesnot get limited 
                due to size of int or long int. we can calculate any big number operation with this method.
                In this project I have used Linked list concept where each number is first converted to a list of node where each node 
                holds digit in the base of 10000 except the first node which can or canot be a base of 10000.
                In this project the I have implemented addition,multiplication,subtraction,division and modulo operation*/