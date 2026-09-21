/**
 * @file id3_writer.c
 * @brief Implementation of functions for writing and editing ID3 tags in MP3 files.
 */

#include <stdio.h>
#include <string.h>
#include "id3_writer.h"
#include "id3_reader.h"
#include "id3_utils.h"
#include<stdlib.h>
/**
 * @brief Writes the ID3 tags to an MP3 file.
 * 
 * @param filename The name of the MP3 file.
 * @param data Pointer to the TagData structure containing the ID3 tags.
 * @return 0 on success, non-zero on failure.
 */
 int get_frame_size(FILE *fp){
   unsigned char f_size[4];
   fread(f_size,sizeof(char),4,fp);
   return (f_size[0]<<24)|(f_size[1]<<16)|(f_size[2]<<8)|(f_size[3]);
 }
 int check_id3_tags_writer(FILE *fp)
 {
   char check[4]="";
   fread(check,sizeof(char),3,fp);
    if(strcmp(check,"ID3")!=0){ 
        printf("ID3 is not matching ");
        return 0;
    }else{
        printf("\t--------- ID3 --------------------------\n\n");
    }
   unsigned char v;
   
    fread(&v,sizeof(char),1,fp);
    if(v == (unsigned)0x03){// cmp according to endianess
        printf("\tID3 v2.3.0 version\n\n");
         return 1;
    }else {
        printf("\tVersion is not matching with v2.3.0\n\n");
        return 0;
    }

 }
 int write_tag_unicode(char *value,int tag_size,FILE *fpwrite,int n){
    int i;
    for(i=0;i<n;i++){
        fwrite(&value[i],sizeof(char),1,fpwrite);
        fseek(fpwrite,1,SEEK_CUR);
    }
    for(int j=i;j<(tag_size/2)-1;j++)
    {
       char temp=0x00;
       fwrite(&temp,sizeof(char),1,fpwrite);
       fseek(fpwrite,1,SEEK_CUR);
    }
    return 1;
 }
int write_tag(char *value,int tag_size,FILE *fpwrite,int n){
    
        fwrite(value,sizeof(char),n,fpwrite);
        //fseek(fpwrite,1,SEEK_CUR);
    
    for(int j=n;j<tag_size-1;j++){
       char temp=0x00;
       fwrite(&temp,sizeof(char),1,fpwrite);
       //fseek(fpwrite,1,SEEK_CUR);
    }
    return 1;
 }
char *check_tag(char *tag)
{
   if(!strcmp(tag,"-t")) return "TIT2";
   else if(!strcmp(tag,"-a")) return "TPE1";
   else if(!strcmp(tag,"-A")) return "TALB";
   else if(!strcmp(tag,"-y")) return "TYER";
   else if(!strcmp(tag,"-g")) return "TCON";
   else if(!strcmp(tag,"-c")) return "TCOM";
   else return NULL;
}
char *check_tag_name(char *tag)
{
   if(!strcmp(tag,"TIT2")) return "TITLE";
   else if(!strcmp(tag,"TPE1")) return "ARTIST";
   else if(!strcmp(tag,"TALB")) return "ALBUM";
   else if(!strcmp(tag,"TYER")) return "YEAR";
   else if(!strcmp(tag,"TCON")) return "GENRE";
   else if(!strcmp(tag,"TCOM")) return "COMPOSER";
   else return NULL;
}
/** TODO: Add documentation as sample given above */
int edit_tag(const char *filename, char *tag, char *value) {
    // TagData *data = create_tag_data();
    FILE *fpwrite;
    if((fpwrite = fopen(filename,"r+"))==NULL){
        printf("\tUNABLE TO OPEN FILE\n");
        return 0;
    }
    if(!check_id3_tags_writer(fpwrite))
    {
        printf("\tERROR READING TAGS \n");
        return 0;
    }
    char *str = check_tag(tag);
    fseek(fpwrite,6,SEEK_SET);
    int meta_size = get_size(fpwrite);
   // printf("----> %d \n",meta_size);
    char *string= "tit2";
    // fwrite(str,1,4,fpwrite);
    // fseek(fpwrite,-4,SEEK_CUR);
    while(ftell(fpwrite)<= meta_size)
    {
        char frame_tag[5];
        fread(frame_tag,1,4,fpwrite);
        frame_tag[4]='\0';
        int frame_size = get_frame_size(fpwrite);
        fseek(fpwrite,2,SEEK_CUR); // flag 
        // printf("frame tag ----> %s str ----->%s\n",frame_tag,str);
        // printf("----> %d\n",frame_size);
        if(!strcmp(frame_tag,str))
        {
                int n=strlen(value);
                unsigned char ch;
                fread(&ch,1,1,fpwrite);
                fseek(fpwrite,0,SEEK_CUR);
                if(ch == 0x01)
                {  
                    if(n > ((frame_size-3)/2)){
                    
                   printf("\tThe new tag value exceeds the available space.\n");
                   return 0;
                   }
                  fseek(fpwrite,2,SEEK_CUR);
                  for(int i=0 ; i < n;i++)
                  {
                    fwrite(&value[i],1,1,fpwrite);
                    fseek(fpwrite,1,SEEK_CUR);
                  }
                  for(int j = n;j<(frame_size/2)-1;j++)
                  {
                    char zero=0x00;
                    fwrite(&zero,1,1,fpwrite);
                   // fwrite(&zero,1,1,fpwrite);
                   }
                    printf("\t---------- CHANGE THE %s ------------\n\n",check_tag_name(str));
                printf("\t%s : %s\n\n",check_tag_name(str),value);
                printf("\t---SUCCESSFULY CHANGED %s------------\n\n",check_tag_name(str));
                  //printf("value---> %s",value);//
                  break;
                }
                else if(ch == 0x00)
                {
                    if(n > (frame_size-1)){
                   printf("\tThe new tag value exceeds the available space.\n");
                   return 0;
                   }
                   for(int i=0 ; i < n;i++)
                  {
                    fwrite(&value[i],1,1,fpwrite);
                   // fseek(fpwrite,1,SEEK_CUR);
                  }
                   fseek(fpwrite,0,SEEK_CUR);
                  for(int j = n;j<(frame_size)-1;j++)
                  {
                    char zero=0x00;
                    fwrite(&zero,1,1,fpwrite);
                   // fwrite(&zero,1,1,fpwrite);
                   }
                    printf("\t---------- CHANGE THE %s ------------\n\n",check_tag_name(str));
                printf("\t%s : %s\n\n",check_tag_name(str),value);
                printf("\t---SUCCESSFULY CHANGED %s------------\n\n",check_tag_name(str));
                //    printf("value---> %s",value);
                   break;
                 
                }
                else if(ch==0x02)
                {
                   if(n > ((frame_size-1)/2)){
                    
                     printf("\tThe new tag value exceeds the available space.\n");
                      return 0;
                   }
                 // fseek(fpwrite,2,SEEK_CUR);
                  for(int i=0 ; i < n;i++)
                  {
                    fwrite(&value[i],1,1,fpwrite);
                    fseek(fpwrite,1,SEEK_CUR);
                  }
                  for(int j = n;j<(frame_size/2)-1;j++)
                  {
                    char zero=0x00;
                    fwrite(&zero,1,1,fpwrite);
                   // fwrite(&zero,1,1,fpwrite);
                   }
                    printf("\t---------- CHANGE THE %s ------------\n\n",check_tag_name(str));
                    printf("\t%s : %s\n\n",check_tag_name(str),value);
                    printf("\t---SUCCESSFULY CHANGED %s------------\n\n",check_tag_name(str));
                  //printf("value---> %s",value);//
                  break;

                }
        }       
        else
        {
            fseek(fpwrite,frame_size,SEEK_CUR);
        }    
    
   }
      return 1;
}
