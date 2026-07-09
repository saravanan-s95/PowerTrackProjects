/*👤NAME        : THARSHINI S
  📅DATE        : 09-12-2025
  📝DESCRIPTION : INVERTED SEARCH - SEARCH DATABASE */ 
  
#include "inverted.h"
#include<string.h>

int search_database(Htable *table, char *word)
{
   //validate user input
   if(word==NULL||word[0]=='\0')
   {
    printf("ERROR: Invalid word!\n");
    return FAILURE;
   }

   //compute hash index
   int index=hash_key(word);
   WordNode *temp=table[index].head;

   //search for the word in linked list
   while(temp!=NULL)
   {
    if(strcmp(temp->word,word)==0)
    {
        printf("\nWord '%s' is found in the database and it is present in %d file(s)\n",word,temp->file_count);
        FileNode *f=temp->f_link;
        while(f!=NULL)
        {
            printf("In file: %s %d time(s)\n",f->fname,f->word_count);
            f=f->f_link;
        }
        return SUCCESS;
    }
    temp=temp->w_link;
   }

   //if word not found
   printf("Word '%s' is not found in the database\n",word);
   return FAILURE;
}
