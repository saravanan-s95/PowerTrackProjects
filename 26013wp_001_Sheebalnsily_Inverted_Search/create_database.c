#include "inverted_Search.h"
char *file_name;
void create_database(Flist *f_head, Wlist *head[])
{
    while(f_head)
    {
        //read each file one after the other
        read_datafile(f_head, head, f_head->file_name);
        //move the file head to next node
        f_head = f_head->link;
    }
}
//function to read the content of file and create dtabase for the word
Wlist *read_datafile(Flist *file, Wlist *head[], char *filename)
{
    FILE *fptr = fopen(filename, "r");
    if(fptr == NULL)
       {
    return NULL;
       }
    file_name = filename;
    //validation for open
    char word[WORD_SIZE];
    while(fscanf(fptr, "%s", word) != EOF)
    {
        int flag = 1;
        //find the index
        int index = hash_function(word);
        if(head[index] != NULL)
        {
            Wlist *temp = head[index];
            while(temp)
            {
                if(strcmp(temp->word, word) == 0)
                {
                    update_word_count(&temp, filename);
                    flag = 0;
                    break;
                }
                temp = temp->link;
            }
        }
        if(flag == 1)
            insert_at_last(&head[index], word);
    }
    fclose(fptr);
    return NULL;
}
int update_word_count(Wlist **head, char *filename)
{
    Ltable *temp = (*head)->Tlink;
    Ltable *prev = NULL;

    /* Check whether the file already exists */
    while(temp != NULL)
    {
        if(strcmp(temp->file_name, filename) == 0)
        {
            /* Same word in same file */
            temp->word_count++;
            return SUCCESS;
        }

        prev = temp;
        temp = temp->table_link;
    }

    /* Same word but in a new file */
    Ltable *new = malloc(sizeof(Ltable));

    if(new == NULL)
    {
        return FAILURE;
    }

    new->word_count = 1;
    strcpy(new->file_name, filename);
    new->table_link = NULL;

    /* Add new file node at the end */
    if(prev == NULL)
    {
        (*head)->Tlink = new;
    }
    else
    {
        prev->table_link = new;
    }

    (*head)->file_count++;

    return SUCCESS;
}
int hash_function(const char *word)
{
    if(isalpha((unsigned char)word[0]))
    {
        return tolower((unsigned char)word[0]) - 'a';
    }

    return 26;
}