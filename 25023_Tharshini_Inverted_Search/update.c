/*👤NAME        : THARSHINI S
  📅DATE        : 09-12-2025
  📝DESCRIPTION : INVERTED SEARCH - UPDATE DATABASE */ 
  
#include "inverted.h"
#include<string.h>

int update_database(Htable *table, char *fname, Inputfile **list)
{
   //validate .txt extension
   int len=strlen(fname);
   if(len<4||strcmp(fname+len-4,".txt")!=0)
   {
    printf("ERROR: %s is not a .txt file\n",fname);
    return FAILURE;
   }
   
    //Check if file is already exists in database
    if(is_file_in_hashtable(table,fname))
    {
        printf("File '%s' already exists in database. Skipping...\n",fname);
        return FILE_REPEATED;
    }

   //try opening the file
    FILE *fptr=fopen(fname, "r");
    if(fptr==NULL)
    {
        printf("ERROR: Cannot open file '%s'\n",fname);
        return FILE_NOT_FOUND;
    }

    //Check if file is empty
    fseek(fptr, 0, SEEK_END);
    if(ftell(fptr)==0)
    {
        printf("ERROR: File '%s' is empty\n",fname);
        fclose(fptr);
        return FILE_EMPTY;
    }
    rewind(fptr);


    //add file to the list
    Inputfile *new=malloc(sizeof(Inputfile));
    if(new==NULL)
    {
        printf("ERROR: Memory allocation failed!\n");
        fclose(fptr);
        return FAILURE;
    }
    strcpy(new->fname,fname);
    new->link=*list;
    *list=new;

    //insert words into hashtable
    char word[MAXWORD];
    int ch;
    int w_index=0;

    //read file char by char
    while((ch=fgetc(fptr))!=EOF)
    {
        //if alphabet,build word
        if((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')||(ch>='0'&&ch<='9')||ch=='#'||ch=='$'||ch=='&'||ch=='^'||ch=='!')
        {
            if(w_index < MAXWORD-1)
            {
            word[w_index++]=ch;
            }
        }
        else
        {
            //non alphabet,word ended
            if(w_index > 0)
            {
                word[w_index]='\0';
                int index=hash_key(word);
                insert_word_to_hashtable(table,word,fname,index);
                w_index=0;
            }
        }
    }

        //handle last word if file ends without punctuation
        if(w_index > 0)
        {
            word[w_index]='\0';
            int index=hash_key(word);
            insert_word_to_hashtable(table,word,fname,index);
        }
        fclose(fptr);


        printf("File '%s' updated successfully!\n",fname);
        return SUCCESS;

}
