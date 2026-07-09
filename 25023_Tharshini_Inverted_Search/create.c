/*👤NAME        : THARSHINI S
  📅DATE        : 09-12-2025
  📝DESCRIPTION : INVERTED SEARCH - CREATE DATABASE */ 
  
#include "inverted.h"

int create_database(Htable *table, Inputfile *list)
{
    if(list==NULL)
    {
        printf("ERROR: No input files found!\n");
        return FAILURE;
    }
    Inputfile *temp=list;
    FILE *fptr;
    char word[MAXWORD];
    int w_index;

    while(temp!=NULL)
    {
        fptr=fopen(temp->fname,"r");
        if(fptr==NULL)
        {
            printf("ERROR: Cannot open file: %s\n",temp->fname);
            return FILE_NOT_FOUND;
        }
        int ch;
        int file_empty=1;
        w_index=0;

        //read file char by char
        while((ch=fgetc(fptr))!=EOF)
        {
            file_empty=0;//As soon as one char is read, file is not empty

            //if alphabet,build word
            if((ch>='A'&&ch<='Z')||(ch>='a' && ch<='z')||(ch>='0' && ch<='9')||ch=='#'||ch=='$'||ch=='&'||ch=='^'||ch=='!')
            {
                if(w_index < MAXWORD-1)
                {
                    word[w_index++]=ch;
                }
            }
            else
            {
                //non alphabet,word ended
                if(w_index > 0)
                {
                    word[w_index]='\0';
                    int index=hash_key(word);
                    insert_word_to_hashtable(table, word, temp->fname, index);

                    w_index=0; //reset
                }
            }
        }

        //if file ends with a word(no delimiter)
        if(w_index > 0)
        {
            word[w_index]='\0';
            int index=hash_key(word);
            insert_word_to_hashtable(table, word, temp->fname, index);
        }
        fclose(fptr);

        if(file_empty)
        {
            printf("WARNING: File '%s' is empty. Skipping...\n",temp->fname);
        }
        else
        {
            printf("File '%s' processed successfully.\n",temp->fname);
        }
        temp=temp->link;
    }
    return SUCCESS;
}
