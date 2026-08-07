

#include "inverted_search.h"


void save_database(mnode *arr[])
{
  char filename[20];
  printf("\tEnter the filename to save \n\t:");
  scanf("%s",filename);
  if(strstr(filename,".txt")!=NULL)
  {
     FILE *fptr = fopen(filename,"w");
      fprintf(fptr,"-----------------------------------------------------------------------------------------------\n");
      fprintf(fptr,"|Index\t\tWord\t\t\t filecount\t\t  filename\t \t\t   wordcount   |\n");
      fprintf(fptr,"-----------------------------------------------------------------------------------------------\n");
      for(int i=0;i<27;i++)
      {

        if(arr[i]!= NULL)
        {
            mnode *temp = arr[i];
           while(temp != NULL)
           {
               fprintf(fptr,"|%-5d\t\t%-13s    \t\t%-9d\t",i + 1,temp->word,temp->filecount);
               snode *stemp = temp->slink;
               int j =0;
               while(stemp != NULL)
               {
                if(j == 0){
                    fprintf(fptr,"  %-16s %8d   |\n",stemp->fname,stemp->wc);
                }else{
                    fprintf(fptr,"|\t\t\t\t\t\t                          %-16s %8d   |\n",stemp->fname,stemp->wc);
                }
                j++;
                stemp = stemp->slink;
               }
               temp = temp->mlink;
               fprintf(fptr,"-----------------------------------------------------------------------------------------------\n");
           }
         }
     }
  }
  else
  {
    printf("\tEnter the filename with .txt extn\n");
  }
}