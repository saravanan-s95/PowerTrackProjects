

#include "inverted_search.h"

void update_database(mnode *arr[],Slist **head)
{
   char filename[20];
   printf("\tEnter the new file name  to be included\n\t:");
   scanf("%19s",filename);
   int flag =0;
    if(strstr(filename,".txt")!=NULL)
		 {
            FILE *fp;
			if((fp = fopen(filename,"r"))==NULL){
				  printf("\tunable to open file\n");
                  flag = 1;
                  goto label;
			}else{
				  int ch = fgetc(fp);
                  if (ch == EOF) {
                        
                          flag = 1;
                          goto label;
                          // File is empty
                    }
				  if(*head == NULL){
					insert_at_last(head,filename);
				  }else{
					  int cnt =0;
					  Slist *temp = *head;
					  while(temp){
						if(!strcmp(temp->filename,filename)){
							cnt =1;
							break;
						}
						temp = temp->link;
					  }
					  if(!cnt){
						  insert_at_last(head,filename);
					  }
                      else
                      {
                        printf("\t The filename already exists\n");
                      }
				  }
			}
		 }
         label:
         if(flag ==1){
            printf("\tUnable to update the file please check the filename correctly\n");
         }else{
            create_database(arr,*head);
            printf("\tUpdated successfully\n");
         }
}
