/*👤NAME        : THARSHINI S
  📅DATE        : 09-12-2025
  📝DESCRIPTION : INVERTED SEARCH - VALIDATION FUNCTIONS */ 
  
#include "inverted.h"
#include <string.h>

int validate_args(int argc,char *argv[],Inputfile **list)
{
    if(argc<2)
    {
        printf("USAGE: ./a.out <file1> <file2> ...\n");
        return FAILURE;
    }

    for(int i=1;i<argc;i++)
    {
        //check extension
        if(is_textfile(argv[i])==FAILURE)
        {
            printf("Skipping invalid file:(not .txt): %s\n",argv[i]);
            continue;
        }

        //check if file exists
        FILE *fptr=fopen(argv[i],"r");
        if(fptr==NULL)
        {
            printf("ERROR: File not found: %s\n",argv[i]);
            continue;
        }

        //check if file is empty
        fseek(fptr,0,SEEK_END);
        long size=ftell(fptr);
        fclose(fptr);
        if(size==0)
        {
            printf("ERROR: Empty file skipped: %s\n",argv[i]);
            continue;
        }

        //check repeated file in list
        Inputfile *temp=*list;
        int repeated=0;
        while(temp)
        {
            if(strcmp(temp->fname,argv[i])==0)
            {
                printf("ERROR: Repeated file ignored: %s\n",argv[i]);
               // return FILE_REPEATED;
               repeated=1;
               break;
            }
            temp=temp->link;
        }
        if(repeated)
          continue;

        //add file to linked list
        Inputfile *new=malloc(sizeof(Inputfile));
        if(new==NULL)
        {
            printf("ERROR: Memory allocation failed!\n");
            return FAILURE;
        }

        strcpy(new->fname,argv[i]);
        new->link=*list;
        *list=new;
    }
      if(*list==NULL)
        {
            printf("ERROR: No valid .txt files found!\n");
            return FAILURE;
        }
    return SUCCESS;
}

int is_textfile(const char *fname)
{
    int len=strlen(fname);
    if(len<4)
    {
        return FAILURE;
    }
    //check last 4 characters for .txt
    if(fname[len-4]=='.' && fname[len-3]=='t' && fname[len-2]=='x' && fname[len-1]=='t')
    {
        return 1;
    }
    return FAILURE;
}