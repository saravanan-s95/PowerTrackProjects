#include "types.h"
#include "lexical.h"

Status storing_cla_fnames_to_LL(char *argv[], File_List **head)
{
    int i = 1;

    while (argv[i])
    {
        // Checking validation of file name
        Status val_f_name = validate_fname(argv[i]);

        if (val_f_name != e_success)
        {
            i++;
            continue;
        }
        // Creation of new node
        File_List *new = malloc(sizeof(File_List));
        if (new == NULL)
        {
            printf("Node cannot be created for storing file names\n");
            return e_failure;
        }

        // Inserting data into node
        strcpy(new->f_name, argv[i]);
        new->link = NULL;

        Status ins_last = insert_last(head, new);

        if (ins_last != e_success)
        {
            printf("ERROR: Insert Last operation cannot be performed while storing file name into list from CLA\n");
            return e_failure;
        }

        i++;
    }

    return e_success;
}