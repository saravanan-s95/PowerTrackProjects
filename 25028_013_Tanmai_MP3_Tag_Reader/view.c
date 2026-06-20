#include<stdio.h>
#include<string.h>
#include<stdlib.h>
/*
 * Function : filename()
 * Purpose  : Validates whether the file
 *            has .mp3 extension or not.
 */
int filename(char *str)
{
    int len=strlen(str);
    if(len<4)
    {
        return 0;
    }
    char *ext = str + len - 4; 
    if (strcmp(ext,".mp3")==0) 
    { 
        printf("valid file\n");
        return 1;
    }
    else
    {
        printf("Invalid mp3 file\n");
        return 0;
    }
}
/*
 * Function : endian_size()
 * Purpose  : Converts big-endian value
 *            to little-endian format.
 */
int endian_size(int size)
{
    unsigned char *size1=(unsigned char *)&size;
    return (size1[0]<<24)|(size1[1]<<16)|(size1[2]<<8)|size1[3];
}
/*
 * Function : view()
 * Purpose  : Reads and displays ID3 tags
 *            from MP3 file.
 */
void view(char *filename)
{
    FILE *fptr=NULL;
    fptr=fopen(filename,"rb");
    if(fptr==NULL)
    {
            printf("file doesn't exist");
            exit(1);
        }
        // User friendly tag names
        char *tags[6]={"TITLE","ARTIST","ALBUM","YEAR","CONTENT","COMPOSER"};
        // Actual ID3 frame identifiers
        char *Tags[6]={"TIT2","TPE1","TALB","TYER","TCON","COMM"};
        char *TAGS[6]={NULL};
         // Skip ID3 header
        fseek(fptr,10,SEEK_SET);
        for(int i=0;i<6;i++)
        {
        char tag[5]; 
        // Read frame ID       
        fread(tag,4,1,fptr);
        tag[4]='\0';
         // Read frame size
        int size;
        fread(&size,4,1,fptr);
        size=endian_size(size);
        //Skip frame flags
        fseek(fptr,2,SEEK_CUR);
         // Allocate memory for frame data
        char *data=malloc(size+1);
        fread(data,size,1,fptr);
        data[size]='\0'; 
        // Match tag and store data
        for(int j=0;j<6;j++)
        {
            if(!strcmp(tag,Tags[j]))
            {
                TAGS[j]=malloc(strlen(data+1)+1); //ex:"hello" with data+1 it prints ello null characters will not taken   
                strcpy(TAGS[j],data+1);          //in lucky case it will print other wise segmentation fault
            }
        }
        }
         // Display metadata
        for(int k=0;k<6;k++)
        {
           printf("%-10s:%s\n",tags[k],TAGS[k]);

        }
        printf("SUCCESSFULLY READING\n");
        fclose(fptr);
}