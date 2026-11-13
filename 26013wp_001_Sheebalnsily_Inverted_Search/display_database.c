#include "inverted_Search.h"
void display_database(Wlist *head[])
{
    //Display the content of all the head
    printf("Index\tWord\tFile Count\tFilename\tWord Count\n");
    for(int i = 0; i < 27;i++)
    {
        //print alphabetically one after the other
        if(head[i] != NULL)
        {
            print_word_count(head[i]);
        }
    }
}
int print_word_count(Wlist *head)
{
    //traverse through the Wlst
    while(head != NULL)
    {
        printf("%d\t%s\t%d\t", tolower(head->word[0]) %97, head->word, head->file_count);
        //traverse through the Tlink table
        Ltable *Thead = head->Tlink;
        while(Thead)
        {
            printf("%s\t%d\n", Thead->file_name, Thead->word_count);
            Thead = Thead->table_link;
        }
        printf("\n");
        head = head->link;
    }
    return 0;
}