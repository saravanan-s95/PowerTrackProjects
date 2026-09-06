
#include "inverted_search.h"
void display_database(mnode *arr[])
{
    printf("\t-----------------------------------------------------------------------------------------------\n");
    printf("\t|Index\t\tWord\t\t\t filecount\t\t  filename\t  wordcount   |\n");
    printf("\t-----------------------------------------------------------------------------------------------\n");
    for(int i=0;i<27;i++)
    {

        if(arr[i]!= NULL)
        {
            mnode *temp = arr[i];
           while(temp != NULL)
           {
               printf("\t|%-5d\t\t%-13s    \t\t%-9d\t",i + 1,temp->word,temp->filecount);
               snode *stemp = temp->slink;
               int j =0;
               while(stemp != NULL)
               {
                if(j == 0){
                    printf("  %-16s %8d   |\n",stemp->fname,stemp->wc);
                }else{
                     printf("\t|\t\t\t\t\t\t                  %-16s %8d   |\n",stemp->fname,stemp->wc);
                }
                j++;
                stemp = stemp->slink;
               }
               temp = temp->mlink;
               printf("\t-----------------------------------------------------------------------------------------------\n");
           }
        }
    }
}