#include "inverted_Search.h"

int search(Wlist *head, char *word)
{
    //1. check whether the list is empty or not
    if(head == NULL)
    {
        printf("List is empty\n");
        return 0;
    }

    //not empty
    while(head)
    {
        //compare each node word with the given word
        if(!(strcmp(head->word, word)))
        {
            printf("Word %s is present in %d files\n", word, head->file_count);

            Ltable *Thead = head->Tlink;

            while(Thead)
            {
                printf("In file %s %d\n", Thead->file_name, Thead->word_count);
                Thead = Thead->table_link;
            }

            return SUCCESS;
        }

        head = head->link;
    }

    //if the word is not found
    printf("Search word not found\n");

    return FAILURE;
}