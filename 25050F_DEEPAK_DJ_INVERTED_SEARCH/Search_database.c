#include "inverted_search.h"

void search_database(mnode *arr[])
{
   printf("\tPlease enter the word you want to search\n\t:");
   char word[20];
   scanf("%19s",word);
   int idx = tolower(word[0]) - 'a';
   if(arr[idx]!= NULL)
   {
       mnode *temp = arr[idx];
       int flag =0;
       while(temp != NULL){
         if(strcmp(temp->word,word)==0)
         {
            flag =1;
            break;
         }
         else
             temp = temp->mlink;
       }
       if(flag == 1){
        printf("\tThe word \"%s\" is present in %d files\n",temp->word,temp->filecount);
        snode *stemp = temp->slink;
        while(stemp != NULL){
            printf("\t%s ----> %d\n",stemp->fname,stemp->wc);
            stemp = stemp->slink;
        }
       }
   }
   else
   {
    printf("\tThe word is not found\n");
   }
}