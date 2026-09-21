
#include "inverted_search.h"


void insert_at_last(Slist **head, char *str )
{
     Slist *end = malloc(sizeof(Slist));
     strcpy(end->filename,str);
     end->link = NULL;
     if(*head == NULL){
         *head = end;
         return ;
     }
     Slist *temp= *head;
     while(temp->link != NULL){
         temp = temp->link;
     }
     temp->link=end;
     return ;
}

int read_and_validate(int argc,char *argv[],Slist **head)
{
      for(int i = 1;i<argc;i++)
	  {
		 if(strstr(argv[i],".txt")!=NULL)
		 {
            FILE *fp;
			if((fp = fopen(argv[i],"r"))==NULL){
				  return 0;
			}else{
				  int ch = fgetc(fp);
                  if (ch == EOF) {
                        
                           continue ;// File is empty
                    }
				  if(*head == NULL){
					insert_at_last(head,argv[i]);
				  }else{
					  int cnt =0;
					  Slist *temp = *head;
					  while(temp){
						if(!strcmp(temp->filename,argv[i])){
							cnt =1;
							break;
						}
						temp = temp->link;
					  }
					  if(!cnt){
						  insert_at_last(head,argv[i]);
					  }
				  }
			}
		 }
	  }
	  if(*head == NULL) return 0;
    return 1;
}

int main(int argc, char *argv[])
{
	/* Define the main function here */
	mnode *arr[27]={NULL};
    Slist *head = NULL;
	if(!(argc >= 2))
	{
	    printf("\tEnter : ./a.out filename.extn ..\n");
		return 0;
	}

   if(!read_and_validate(argc,argv,&head))
   {
	   printf("\tENTER like : ./a.out filename.txt ...\n");
	   printf("\t Enter Only .txt files\n");
   }
    
      while(1)
	  {
		printf("\tEnter the choice\n");
		printf("\t1. create database\n\t2. display database\n\t3. Search database\n\t4. Update database\n\t5. Save database\n\t6. Exit\n\t:");
		int choice;
		scanf("%d",&choice);
		switch(choice)
		{
			case 1: 
			     create_database(arr,head); 
				 break;
			case 2:
			     display_database(arr);
				 break;
			case 3:
			      search_database(arr);
				  break;
			case 4:
			      update_database(arr,&head);
				 break;
			case 5:
				 save_database(arr);
				  break;
		    case 6:
			     return 0;
				 break;
			default : printf("\tENTER THE CORRECT OPTION\n");
			          break;
		}
	  }

   return 0;
}

