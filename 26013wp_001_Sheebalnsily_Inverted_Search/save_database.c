#include "inverted_Search.h"

int save_database(Wlist *head[])
{
    char file_name[FNAME_SIZE];

    printf("Enter the filename: ");
    scanf("%s", file_name);

    FILE *fptr = fopen(file_name, "w");

    if(fptr == NULL)
    {
        printf("Unable to open file\n");
        return FAILURE;
    }

    for(int i = 0; i < 27; i++)
    {
        if(head[i] != NULL)
        {
            write_databasefile(head[i], &fptr);
        }
    }

    fclose(fptr);

    return SUCCESS;
}

void write_databasefile(Wlist *head, FILE **databasefile)
{
    while(head != NULL)
    {
        fprintf(*databasefile, "#%d;%s;%d;",
                hash_function(head->word),
                head->word,
                head->file_count);

        Ltable *temp = head->Tlink;

        while(temp != NULL)
        {
            fprintf(*databasefile, "%s;%d;",
                    temp->file_name,
                    temp->word_count);

            temp = temp->table_link;
        }

        fprintf(*databasefile, "#\n");

        head = head->link;
    }
}