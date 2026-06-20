#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int endian_size(int size);
/*
 * Function : edit()
 * Purpose  : Modifies selected MP3 tag
 *            and updates file contents.
 */
void edit(char *filename)
{
    FILE *fptr=NULL;
    fptr=fopen(filename,"rb");
    if(fptr==NULL)
    {
        printf("ERROR: files doesn't exist\n");
        return;
    } 
    // Create temporary output file
    FILE *fptr_output=NULL;
    fptr_output=fopen("newfile.mp3","wb");
    if(fptr_output==NULL)
    {
        printf("ERROR1: files doesn't exist\n");
        fclose(fptr);
        return;      
    }
    // Copy ID3 header (10 bytes)
    char header[10];
    fread(header,10,1,fptr);
    fwrite(header,10,1,fptr_output);
    printf("copied first 10 bits successfully\n");
    char *tags[6]={"TITLE","ARTIST","ALBUM","YEAR","CONTENT","COMPOSER"};
    char *Tags[6]={"TIT2","TPE1","TALB","TYER","TCON","COMM"};
    char edit_tag[5];
    printf("Enter tag to edit(TIT2,TPE1,TALB,TYER,TCON,COMM):");
    scanf("%s",edit_tag);
    for(int i=0;i<6;i++)
    {
        char tag[5];
        fread(tag,4,1,fptr);
        tag[4]='\0';
        unsigned int my_size;
        fread(&my_size,4,1,fptr);
        unsigned int size=endian_size(my_size);
        char flags[2];
        fread(flags,2,1,fptr);
        char *data = malloc(size + 1);
        fread(data, 1, size, fptr);
        data[size] = '\0';
        if(strcmp(tag,edit_tag)==0)
        {
            char new_data[100];
            printf("Enter new data for %s: ",edit_tag);
            scanf(" %[^\n]",new_data);
            int new_size=strlen(new_data)+1;
            int new_size_be=endian_size(new_size);
            fwrite(tag,1,4,fptr_output);           
            fwrite(&new_size_be,4,1,fptr_output);
            fwrite(flags,2,1,fptr_output);
            fputc(0,fptr_output);  //while giving the edit name if size less than the allocated it will filled with '\0'
            fwrite(new_data,1,strlen(new_data),fptr_output);  
            //printf("%s:%s\n",tags[i],new_data);          
        }
        else
        {
            fwrite(tag, 1, 4, fptr_output);
            fwrite(&my_size, 1, 4, fptr_output);
            fwrite(flags, 1, 2, fptr_output);
            fwrite(data, 1, size, fptr_output);
            //printf("%s:%s\n",tags[i],data+1);
        }
        free(data);
    }
    fclose(fptr);
    fclose(fptr_output);
    remove(filename);
    rename("newfile.mp3", filename);
    printf("EDITING SUCCESSFULLY\n");
}