/*👤NAME        : THARSHINI S
  📅DATE        : 09-12-2025
  📝DESCRIPTION : INVERTED SEARCH - SAVE DATABASE */ 
  
#include "inverted.h"

int save_database(Htable *table,Inputfile *list,char *name)
{
    //Check .txt extension
    if(!is_textfile(name))
    {
        printf("ERROR: Output file must be a .txt file!\n");
        return FAILURE;
    }

    //Prevent overwriting input files
    if(is_file_in_list(list, name))
    {
        printf("ERROR: Cannot save database into input file \"%s\"\n",name);
        printf("Please choose another file name like backup.txt\n");
        return FILE_REPEATED;
    }
   //try opening the output file
    FILE *fptr=fopen(name,"w");
    if(fptr==NULL)
    {
        printf("ERROR: Cannot create output file %s\n",name);
        return FILE_NOT_FOUND;
    }
    //write hash table contents
    for(int i=0;i<SIZE;i++)
    {
        WordNode *wnode=table[i].head;
        if(wnode==NULL)
        {
            continue;
        }

        while(wnode)
        {
            fprintf(fptr,"#%d;\n",i);//index
            fprintf(fptr,"%s;%d;",wnode->word,wnode->file_count);//word+file count

            FileNode *fnode=wnode->f_link;
            while(fnode)
            {
                fprintf(fptr,"%s;%d;",fnode->fname,fnode->word_count);
                fnode=fnode->f_link;
            }
            fprintf(fptr,"\n");
            wnode=wnode->w_link;
        }
    }

    fclose(fptr);
    return SUCCESS;
}
