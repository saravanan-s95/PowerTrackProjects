#include "inverter.h"
#include "supportFunction.c"
#include "createDatabase.c"
#include "search.c"
#include "update.c"
#include "savedatabase.c"
#include "display.c"

int main(int argc, char *argv[])
{
    flist *head_f = NULL;
    hash_t arr[27];
    // intilialize hash table
    for (int i = 0; i < 27; i++)
    {
        arr[i].index = i;
        arr[i].link = NULL;
    }
    printf("Hash Table created Succesfully.\n");

    if (argc < 2)
    {
        printf("You have not proivide Input Files\n");
        return 0;
    }

    // validate input and store in linked list
    for (int i = 1; i < argc; i++)
    {
        if ((validateInput(head_f, argv[i]) == SUCCESS) && (Duplicate_find(argv[i], head_f) == SUCCESS) && (File_present_or_not_empty(argv[i]) == SUCCESS))
        {
            insert_last(&head_f, argv[i]);
            printf("%s added successfully into the file linked list.\n", argv[i]);
        }
    }

    printf("Validate File List : ");
    print_list(head_f);

    while (1)
    {
        int choice;
        printf("------------------------------------------------------\n");
        printf("1. Create Database\n");
        printf("2. Display Database\n");
        printf("3. Search Database\n");
        printf("4. Update Database\n");
        printf("5. Save Database\n");
        printf("6. Exit\n");
        printf("------------------------------------------------------\n");

        printf("Enter the choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            if (createdatabase(arr, head_f) == SUCCESS)
            {
                printf("Database Created Successfully !!!\n");
            }
            else
            {
                printf("Database Creation Failed !!!\n");
            }
            break;
        case 2:
            display(arr);
            break;
        case 3:
            char find[25];
            printf("Enter word to search: ");
            scanf("%s",find);
            if(searchdatabase(arr,find) == FAILURE)
            {
                printf("%s is not found in database\n",find);
            }
            break;
        case 4:
            update_database(arr,head_f);
            break;
        case 5:
            savedatabase(arr);
            break;
        case 6:
            printf("--------------------------------------------------------------------------\n");
            printf("Thank You for using application for searching the data in database!!!\n\n");
            printf("--------------------------------------------------------------------------\n");
            printf("Exit Successfully from Application!!\n");
            exit(0);
            break;
        default:
            printf("You Choice Wrong Opetion\n");
            break;
        }
    }
    return 0;
}