#include "shell.h"
extern struct input *head;
extern int pid;
extern char input_str[25];

void insert_first()
{
    //create new node
    struct input *new = malloc(sizeof(struct input));
    //initialise node
    new->pid = pid;
    strcpy(new->command , input_str);
    new->link = head;
    //update head
    head = new;
    
}

void delete_first(){
    struct  input *temp = head;
    head = head->link;
    free(temp);
}

void print_list()
{
    struct  input *temp = head;
    int i=1;

    while(temp != NULL)
    {
        printf("[%d]\tStopped\t\t%s\n",i , temp->command);
        i++;
        temp = temp->link;
    }
}