/*👤NAME        : THARSHINI S
  📅DATE        : 09-12-2025
  📝DESCRIPTION : INVERTED SEARCH - HELPER FUNCTIONS */ 
  
#include "inverted.h"
#include<string.h>

int hash_key(const char *word)
{
    char ch=word[0];
    return (unsigned char)ch;
}


void insert_word_to_hashtable(Htable *table, char *word, char *fname, int index)
{
    WordNode *wtemp=table[index].head;
    WordNode *prev=NULL;

    //search if word exist
    while(wtemp!=NULL)
    {
        //string compare for word
        int i=0;
        while(word[i]!='\0' && wtemp->word[i]!='\0' && word[i]==wtemp->word[i])
        {
            i++;
        }

        if(word[i]=='\0'&&wtemp->word[i]=='\0')
        {
            break;//word found
        }

        prev=wtemp;
        wtemp=wtemp->w_link;
    }

    //if word does not exists->create a new wordnode and insert at head

    if(wtemp==NULL)
    {
        wtemp=(WordNode *)malloc(sizeof(WordNode));
        if(wtemp==NULL)
        {
            printf("ERROR: Memory allocation failed!\n");
            return;
        }

        //copy word into wtemp->word
        int i=0;
        while(word[i]!='\0')
        {
            wtemp->word[i]=word[i];
            i++;
        }
        wtemp->word[i]='\0';

        wtemp->file_count=0;
        wtemp->f_link=NULL;

        //insert at head
        wtemp->w_link=table[index].head;
        table[index].head=wtemp;
    }

        //check if file already exists
        FileNode *ftemp=wtemp->f_link;
        FileNode *fprev=NULL;

        while(ftemp!=NULL)
        {
            //string compare
            int i=0;
            while(fname[i]!='\0' && ftemp->fname[i]!='\0' && fname[i]==ftemp->fname[i])
            {
              i++;
            }

            if(fname[i]=='\0' && ftemp->fname[i]=='\0')
            {
              //file found -> just increase count
               ftemp->word_count++;
               return;
            }
            fprev=ftemp;
            ftemp=ftemp->f_link;
        }

        //if file not found create new filenode
        FileNode *new_file=(FileNode *)malloc(sizeof(FileNode));
        if(new_file==NULL)
        {
            printf("ERROR: Memory allocation failed!\n");
            return;
        }
        //manual copy fname
        int j = 0;
        while(fname[j]!='\0')
        {
            new_file->fname[j]=fname[j];
            j++;
        }
        new_file->fname[j]='\0';
        new_file->word_count=1;
        new_file->f_link=NULL;
        //Insert file node at start of file list
        new_file->f_link=wtemp->f_link;
        wtemp->f_link=new_file;

        //Increase file_count
        wtemp->file_count++;

}


int is_file_in_hashtable(Htable *table,const char *fname)
{
    for(int i=0;i<SIZE;i++)
    {
       WordNode *wtemp=table[i].head;

       while(wtemp)
       {
         FileNode *ftemp=wtemp->f_link;

         while(ftemp)
         {
            if(strcmp(ftemp->fname,fname)==0)
            {
                return 1;//file already present in DB
            }
            ftemp=ftemp->f_link;
         }
         wtemp=wtemp->w_link;
       }
    }
    return 0; //file not found in DB
}

int is_file_in_list(Inputfile *list, const char *fname)
{
    Inputfile *temp=list;
    while(temp)
    {
        if(strcmp(temp->fname,fname)==0)
            return 1;   //file exists in input list
        temp=temp->link;
    }
    return 0;
}


void free_filelist(Inputfile **list)
{
    Inputfile *temp;
    while(*list)
    {
        temp=*list;
        *list=(*list)->link;
        free(temp);
    }
}


void free_database(Htable *table)
{
    for(int i=0;i<SIZE;i++)
    {
      WordNode *wtemp=table[i].head;
      while(wtemp)
      {
         FileNode *ftemp=wtemp->f_link;
         while(ftemp)
         {
            FileNode *f=ftemp;
            ftemp=ftemp->f_link;
            free(f);
         }
         WordNode *W=wtemp;
         wtemp=wtemp->w_link;
         free(W);
      }
    }
}