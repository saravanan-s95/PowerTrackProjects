#include "../include/header.h"

void insertatlast(line**head,line**tail,char* str){
    line* new=malloc(sizeof(line));
    new->next=NULL;
    new->prev=NULL;
    strcpy(new->str,str);
    if(*head==NULL){
        *head=*tail=new;
        return;
    }
    else{
        new->prev=*tail;
        (*tail)->next=new;
        *tail=new;
        return;
    }
}

void push(Stack** top,Action action){
    Stack* new=malloc(sizeof(Stack));
    new->action=action;
    new->next=*top;
    *top=new;
}

int pop(Stack** top,Action* action){
    if(*top==NULL)
        return NO_OPERATION;
    *action =(*top)->action;
    *top=(*top)->next;   
    return SUCCESS;  
}