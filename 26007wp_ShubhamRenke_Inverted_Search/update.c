#include "inverter.h"

int update_database(hash_t *arr, flist *head)
{
    char file_name[25];
    printf("Enter the Filename only .txt : ");
    scanf("%s", file_name);

    if ((validateInput(head, file_name) == SUCCESS) && (Duplicate_find(file_name, head) == SUCCESS) && (File_present_or_not_empty(file_name) == SUCCESS))
    {
        insert_last(&head, file_name);
        printf("%s Updated successfully into the file linked list.\n", file_name);
    }
    else
    {
        printf("File not added to File Linked List\n");
        return FAILURE;
    }
    printf("Updated file list is : ");
    print_list(head);

    flist *temp = head;
    while(temp!= NULL)
    {
        if(!strcmp(temp->filename,file_name))
        {
            createdatabase(arr,temp);
            printf("New File content added succesfully in database \n");
            return SUCCESS;
        }
        temp = temp->link;
    }
}