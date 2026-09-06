/***************************************************************************************************************************************************

*Date           :Mon 22 May 2017 14:04:05 IST
*File           :create_database.c
*Title          :To create the database
*Description    :The list of the files can be provided by storing all the file names in another file, FileList the names of the files which 
		:are to be documented are provided by this file. When a file is added or removed, FileList is changed accordingly.
		:So read the file names and start indexing.
****************************************************************************************************************************************************/
#include "inverted_search.h"

snode *create_snode(char filename[])
{
	snode *newsnode = malloc(sizeof(newsnode));
	newsnode->wc = 1;
	newsnode->fname = filename;
	newsnode->slink = NULL;
	return newsnode;
}
mnode *create_mnode(char word[],char filename[]){
	mnode *newmnode = malloc(sizeof(mnode));
	strcpy(newmnode->word,word);
	newmnode->mlink = NULL;
	newmnode->slink = create_snode(filename);
    return newmnode;
}
void create_database(mnode *arr[], Slist *head)
{
	/* Definition here */
    Slist *temp = head;
	 while(temp != NULL)
	 {
		FILE *fp;
		if((fp = fopen(temp->filename,"r"))!=NULL){
          char word[20];
		   while((fscanf(fp,"%19s",word))==1){
                  //index 
				  int idx = tolower(word[0])-'a';
				  if(arr[idx]==NULL)
				  {
                    //create mainnode
                      mnode *newmnode = malloc(sizeof(mnode));
					  newmnode->filecount = 1;
					  strcpy(newmnode->word,word);
					  newmnode->mlink = NULL;
					//create subnode
                      snode *newsnode = malloc(sizeof(snode));
					  newsnode->wc = 1;
					  newsnode->fname = temp->filename;
					  newsnode->slink = NULL;
                      
					  newmnode->slink = newsnode;
					// update arr of index with the new main node 
                     arr[idx] = newmnode;
				  }
				  else
				  {
                     mnode *temp1 = arr[idx];
					 mnode *prev = NULL;
					 int flag =0;
					 while(temp1 != NULL){
                        if(strcmp(temp1->word,word)==0)
						{
                             flag = 1;
							break;
						}
						else
						   prev = temp1;
						   temp1 = temp1->mlink;
					 }
					 if(flag != 1)
					 {
                        //create mainnode
                      mnode *newmnode = malloc(sizeof(mnode));
					  newmnode->filecount = 1;
					  strcpy(newmnode->word,word);
					  newmnode->mlink = NULL;
					//create subnode
                      snode *newsnode = malloc(sizeof(snode));
					  newsnode->wc = 1;
					  newsnode->fname = temp->filename;
					  newsnode->slink = NULL;
                      
					  newmnode->slink = newsnode;
					// update arr of index with the new main node
					   prev->mlink = newmnode;
					 }
					 else
					 {
                        //check file name matching or not 
						snode *stemp = temp1->slink;
						snode *sprev=NULL;
						int sflag =0;
						while(stemp != NULL)
						{
                          if(strcmp(stemp->fname,temp->filename)==0){
							sflag = 1;
							break;
						  }
						  else
						  {
							sprev = stemp;
							stemp = stemp->slink;
						  }
						}
						if(sflag == 1){
							stemp->wc++;
						}
						else{        
							 sprev->slink =  create_snode(temp->filename);
                             temp1->filecount++;
						}
					 }
				  }
		   }    
		}
        temp = temp->link;
	 }
	 printf("\tTHE DATABASE IS SUCCESSFULLY CREATED \n");
}

