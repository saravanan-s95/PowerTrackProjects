/*👤NAME        : THARSHINI S
  📅DATE        : 09-12-2025
  📝DESCRIPTION : INVERTED SEARCH - DISPLAY DATABASE  */ 
  
#include "inverted.h"

void display_database(Htable *table)
{
   printf("\n---------------DATABASE CONTENTS---------------\n");
   
   int empty=1; //to check if database is empty

   for(int i=0;i<SIZE;i++)
   {
    WordNode *wnode=table[i].head;
    if(wnode==NULL)
    {
        continue;
    }

    empty=0; //at least 1 entry exists

    while(wnode)
    {
        printf("[%d]\t[%-6s]\t%d ",i,wnode->word,wnode->file_count);

        FileNode *fnode=wnode->f_link;
        while(fnode)
        {
            printf(" File: %-10s %d",fnode->fname,fnode->word_count);
            fnode=fnode->f_link;
        }
        printf("\n");
        wnode=wnode->w_link;
    }
   }
   if(empty)
   {
    printf("Database is empty. No words found.\n");
   }
   printf("---------------------------------------------------\n");
}
