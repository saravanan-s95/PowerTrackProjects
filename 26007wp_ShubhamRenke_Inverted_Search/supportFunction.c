#include "inverter.h"
#include<string.h>
#include <stdlib.h>

int insert_last(flist **head , char *name)
{
    flist *newNode = malloc(sizeof(flist));
    strcpy(newNode->filename,name);
    newNode->link = NULL;

    if(*head == NULL)
    {
        *head = newNode;
        return SUCCESS;
    }

    flist *temp = *head;

    while(temp->link != NULL)
    {
        temp = temp->link;
    }

    temp->link= newNode;

    return SUCCESS;
}

#include <stdio.h>

int File_present_or_not_empty(char * filename)
{
    FILE *fptr;
    fptr = fopen(filename, "r");
    
    // 1. Check if file is present
    if(fptr == NULL)
    {
        return FAILURE;
    }

    // 2. Check if file is empty
    fseek(fptr, 0, SEEK_END);     // Move the cursor to the end of the file
    int file_size = ftell(fptr);  // Get the current position (which equals the file size in bytes)

    if(file_size == 0)
    {
        fclose(fptr);             // Clean up before returning
        return FAILURE;           // File exists, but it is empty
    }

    fclose(fptr);                 // Clean up before returning
    return SUCCESS;               // File exists and has data
}

int Duplicate_find(char *name,flist *head)
{
    if(head == NULL)
    {
        return SUCCESS;
    }

    while(head != NULL)
    {
        if(!strcmp(head->filename,name))
        {
            printf("Duplicate file Got : %s\n",name);
            return FAILURE;
        }
        head = head->link;
    }
    return SUCCESS;
}

int validateInput(flist *head,char *argv)
{
    if(strstr(argv,".txt")!=NULL)
    {
        return SUCCESS;
    }
    printf("Invalid File Formate : %s\n",argv);
    return FAILURE;
}



int print_list(flist *head)
{
    if(head == NULL)
    {
        printf("List is Empty\n");
        return 0;
    }

    flist *temp = head;

    while(temp != NULL)
    {
        printf("->%s",temp->filename);
        temp = temp->link;
    }
    printf("\n");
    return 0;
}

