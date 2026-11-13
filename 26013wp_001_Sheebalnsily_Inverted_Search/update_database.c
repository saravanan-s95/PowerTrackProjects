#include "inverted_Search.h"

int update_database(Wlist *head[], Flist **f_head)
{
    //prompt user to enter the new file
    char file_name[FNAME_SIZE];
    printf("Enter the file to be added to the file linked list: ");
    scanf("%s", file_name);

    //validation of file entered
    
    int ret_val = isFileEmpty(file_name);

    if(ret_val == FILE_NOTAVAILABLE)
    {
        printf("File %s is not available\n", file_name);
        return FAILURE;
    }
    else if(ret_val == FILE_EMPTY)
    {
        printf("Contents are empty\n");
        printf("Hence we are not adding that file into Linked List\n");
        return FAILURE;
    }

    //single file we are adding to the file linked list
    int result = to_create_list_of_files(f_head, file_name);

    if(result == REPEATATION)
    {
        printf("File %s is already present in the file linked list\n", file_name);
        return REPEATATION;
    }
    else if(result == FAILURE)
    {
        printf("File linked list insertion failed\n");
        return FAILURE;
    }

    printf("Successfully added %s file into Linked List\n", file_name);

    //create the database for the updated file -- bring the f head to the last node
    Flist *temp = *f_head;

    while(temp->link != NULL)
    {
        temp = temp->link;
    }

    create_database(temp, head);

    return SUCCESS;
}