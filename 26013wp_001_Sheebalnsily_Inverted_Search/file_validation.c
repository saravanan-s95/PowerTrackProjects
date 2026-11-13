#include "inverted_Search.h"

void file_validation_n_file_list(Flist **f_head, char *argv[])
{
    int i = 1; //executable file is excuded
    int empty;
    while(argv[i] != NULL)
    {
        empty = isFileEmpty(argv[i]);
        if(empty == FILE_NOTAVAILABLE)
        {
            printf("FIle %s is not avaialable\n", argv[i]);
            printf("Hence we are not adding that file into Linked List\n");
            i++;
            continue;
        }
        else if(empty == FILE_EMPTY)
        {
            printf("Contents are empty\n");
            printf("Hence we are not adding that file into Linked List\n");
            i++;
            continue;
        }
        else
        {
            int ret_val = to_create_list_of_files(f_head, argv[i]);
            if(ret_val == SUCCESS)
            {
                printf("Successfully added %s file into Linked List\n", argv[i]);
            }
            else if(ret_val == REPEATATION)
            {

            }
            else
            {
                printf("Something went wrong!!!!\n");
            }
        }
        i++;
    }
}
//function is to chek file avaialability and content
int isFileEmpty(char *filename)
{
    FILE *fptr = fopen(filename, "r");

    if(fptr == NULL)
    {
        return FILE_NOTAVAILABLE;
    }

    fseek(fptr, 0, SEEK_END);

    if(ftell(fptr) == 0)
    {
        fclose(fptr);
        return FILE_EMPTY;
    }

    fclose(fptr);

    return SUCCESS;
}

//create the file linked list
int to_create_list_of_files(Flist **f_head, char *name)
{
    Flist *new = malloc(sizeof(Flist));

    if(new == NULL)
    {
        return FAILURE;
    }

    strcpy(new->file_name, name);
    new->link = NULL;

    /* If list is empty */
    if(*f_head == NULL)
    {
        *f_head = new;
        return SUCCESS;
    }

    /* Check for duplicate file */
    Flist *temp = *f_head;

    while(temp != NULL)
    {
        if(strcmp(temp->file_name, name) == 0)
        {
            free(new);
            return REPEATATION;
        }

        temp = temp->link;
    }

    /* Move to last node */
    temp = *f_head;

    while(temp->link != NULL)
    {
        temp = temp->link;
    }

    /* Add new node at last */
    temp->link = new;

    return SUCCESS;
}