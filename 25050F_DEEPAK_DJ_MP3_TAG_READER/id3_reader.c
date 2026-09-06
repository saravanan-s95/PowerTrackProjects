/**
 * @file id3_reader.c
 * @brief Implementation of functions for reading ID3 tags from MP3 files.
 */
#define reverse_bits(n) ((n&(0x0f0f0f0f))<<4)|((n&(0xf0f0f0f0))>>4)
#include <stdio.h>
#include <stdlib.h>
#include "id3_reader.h"
#include "id3_utils.h"
#include "error_handling.h"
#include<string.h>
/**
TODO: Add documention as sample given
 */
int get_size(FILE *fp){
       char s[4];
       fread(s,sizeof(char),4,fp);
       int get =0;
       get = (s[0]<< 21) | get;
       get = get| (s[1] << 14);
       get = get | (s[2] << 7) ;
       get = get| (s[3]);
       return get;
}
int check_frame_tags_for_unicode(TagData *mp3,char *tag,int tag_size,FILE *fpread)
{
    if(strcmp(tag,"TPE1")==0){
        mp3->artist_size = (tag_size)+1;
        mp3->artist=malloc((tag_size+1)*sizeof(char));
       
        fread(mp3->artist,sizeof(char),tag_size,fpread);
            //fseek(fpread,1,SEEK_CUR); // ignore NULl
        mp3->artist[(tag_size)]='\0';
        return 1;
    }
    else if(strcmp(tag,"TIT2")==0){
        mp3->title_size = (tag_size)+1;
         mp3->title =malloc((tag_size+1)*sizeof(char));
       
            fread(mp3->title,sizeof(char),tag_size,fpread);
        mp3->title[(tag_size)]='\0';
        return 1;
    }
    else if(strcmp(tag,"TALB")==0)
    {
        mp3->album_size = (tag_size)+1;
         mp3->album =malloc((tag_size+1)*sizeof(char));
       
            fread(mp3->album,sizeof(char),tag_size,fpread);
        mp3->album[(tag_size)]='\0';
        return 1;
    }
    else if(strcmp(tag,"TYER")==0)
    {
        mp3->year_size = (tag_size)+1;
         mp3->year =malloc((tag_size+1)*sizeof(char));
  
            fread(mp3->year,sizeof(char),tag_size,fpread);
        mp3->year[(tag_size)]='\0';
        return 1;
    }
    else if(strcmp(tag,"TCON")==0)
    {
        mp3->genre_size = (tag_size)+1;
         mp3->genre =malloc((tag_size+1)*sizeof(char));
        
            fread(mp3->genre,sizeof(char),tag_size,fpread);
        mp3->genre[(tag_size)]='\0';
        return 1;
    }
    else if(strcmp(tag,"TCOM")==0)
    {
        mp3->composer_size = (tag_size)+1;
         mp3->composer =malloc((tag_size+1)*sizeof(char));
       
            fread(mp3->composer,sizeof(char),tag_size,fpread);
         
        mp3->composer[(tag_size)]='\0';
        return 1;
    }
    else return 0;
}
int check_frame_tags(TagData *mp3,char *tag,int tag_size,FILE *fpread)
{
    if(strcmp(tag,"TPE1")==0){
        mp3->artist_size=(tag_size/2)+1;
        mp3->artist=malloc((tag_size/2)+1*sizeof(char));
        for(int i=0;i<tag_size/2;i++){
            fread(&mp3->artist[i],sizeof(char),1,fpread);
            fseek(fpread,1,SEEK_CUR); // ignore NULL
        }
        mp3->artist[(tag_size/2)]='\0';
        return 1;
    }
    else if(strcmp(tag,"TIT2")==0){
         mp3->title_size=(tag_size/2)+1;
         mp3->title =malloc((tag_size/2)+1*sizeof(char));
        for(int i=0;i<tag_size/2;i++){
            fread(&mp3->title[i],sizeof(char),1,fpread);
            fseek(fpread,1,SEEK_CUR); // ignore NULL
        }
        mp3->title[(tag_size/2)]='\0';
        return 1;
    }
    else if(strcmp(tag,"TALB")==0){
        mp3->album_size=(tag_size/2)+1;
         mp3->album =malloc((tag_size/2)+1*sizeof(char));
        for(int i=0;i<tag_size/2;i++){
            fread(&mp3->album[i],sizeof(char),1,fpread);
            fseek(fpread,1,SEEK_CUR); // ignore NULL
        }
        mp3->album[(tag_size/2)]='\0';
        return 1;
    }
    else if(strcmp(tag,"TYER")==0){
        mp3->year_size = (tag_size/2)+1;
         mp3->year =malloc((tag_size/2)+1*sizeof(char));
        for(int i=0;i<tag_size/2;i++){
            fread(&mp3->year[i],sizeof(char),1,fpread);
            fseek(fpread,1,SEEK_CUR); // ignore NULL
        }
        mp3->year[(tag_size/2)]='\0';
        return 1;
    }
    else if(strcmp(tag,"TCON")==0){
        mp3->genre_size=(tag_size/2)+1;
         mp3->genre =malloc((tag_size/2)+1*sizeof(char));
        for(int i=0;i<tag_size/2;i++){
            fread(&mp3->genre[i],sizeof(char),1,fpread);
            fseek(fpread,1,SEEK_CUR); // ignore NULL
        }
        mp3->genre[(tag_size/2)]='\0';
        return 1;
    }
    else if(strcmp(tag,"TCOM")==0){
        mp3->composer_size = (tag_size/2)+1;
         mp3->composer =malloc((tag_size/2)+1*sizeof(char));
        for(int i=0;i<tag_size/2;i++){
            fread(&mp3->composer[i],sizeof(char),1,fpread);
            fseek(fpread,1,SEEK_CUR); // ignore NULL
        }

        mp3->composer[(tag_size/2)]='\0';
        return 1;
    }
    else return 0;
}

int check_id3_tags(TagData *mp3,FILE *fpread){
    mp3->version = malloc(4);
    mp3->version[3]='\0';
    fread(mp3->version,sizeof(char),3,fpread);
    if(strcmp(mp3->version,"ID3")!=0){ 
        display_error("ID3 is not matching ");
        return 0;
    }else{
        printf("\t--------- ID3 ---------\n");
    }
    unsigned short int v;
    // endianess plays a major role here 
    //file reads 0x0300 but in memory it is stored as 0003
    // fread is dumb it doesnot know how to store in memory it just copies blindly 
    fread(&v,sizeof(char),1,fpread);
    if(v == (unsigned)0x03){// cmp according to endianess
        printf("\tID3 v2.3.0 version\n");
         return 1;
    }else {
        printf("\tVersion is not matching with v2.3.0\n");
        return 0;
    }
}
TagData* read_id3_tags(const char *filename) {
    // Implementation for reading ID3 tags
    FILE *fpread;
    if((fpread=fopen(filename,"r"))==NULL){
        printf("\tUnable to open file\n");
        return NULL;
    }
    TagData *mp3=create_tag_data();
    int cnt=0;
    // checking id3 and version 
   if(!check_id3_tags(mp3,fpread)) return NULL;
    fseek(fpread,6,SEEK_SET);
    int size = get_size(fpread);
    int fp;
   // printf("--> %d",size);

    while((fp=ftell(fpread)) <= size)
    {
      char tag[5]="";
      fread(tag,sizeof(char),4,fpread);
     // printf("\t----> %s\n",tag);
    
      int tag_size = get_size(fpread);
      
     // printf("\t ----> %d\n",tag_size);
      fseek(fpread,2,SEEK_CUR); // flag ignoring
      char ch;
      fread(&ch,sizeof(char),1,fpread);
       // printf("%#X",ch);
        //rames that allow different types of text
        //encoding have a text encoding description byte directly after the
        //frame size.
        //The three byte language field is used to describe the language of the
        //frame's content, according to ISO-639-2 [ISO-639-2].
      if(ch==0x01)
      {
         fseek(fpread,2,SEEK_CUR); // unicode and BOM
         if(check_frame_tags(mp3,tag,tag_size-3,fpread)==1)
          {
              cnt++;
          }
          else
          {
            fseek(fpread,tag_size-3,SEEK_CUR);
          }
     }else if(ch==0x00){
         if(check_frame_tags_for_unicode(mp3,tag,tag_size-1,fpread)==1)
          {
              cnt++;
          }else
          {
            fseek(fpread,tag_size-1,SEEK_CUR);
          }
     }else if(ch==0x02)
      {
         //fseek(fpread,2,SEEK_CUR); // unicode and BOM
         if(check_frame_tags(mp3,tag,tag_size-1,fpread)==1)
          {
              cnt++;
          }
          else
          {
            fseek(fpread,tag_size-3,SEEK_CUR);
          }
      }
      if(cnt >=6) break;
   }
    return mp3;
}

/**
TODO: Add documention as sample given
 */
void display_metadata(const TagData *data) {
    // Implementation for displaying metadata
    printf("\t-----------------------------------------------------\n");
    printf("\tTITLE     ----> ");
    for(int i=0;i<data->title_size;i++){
        if(data->title[i]==0x00) printf(" ");
        else printf("%c",data->title[i]);
    }
    printf("\n");
    printf("\tARTIST    ----> ");
     for(int i=0;i<data->artist_size;i++){
        if(data->artist[i]==0x00) printf(" ");
        else printf("%c",data->artist[i]);
    }
    printf("\n");
    printf("\tALBUM     ----> ");
     for(int i=0;i<data->album_size;i++){
        if(data->album[i]==0x00) printf(" ");
        else printf("%c",data->album[i]);
    }
    printf("\n");
    printf("\tYEAR      ----> ");
     for(int i=0;i<data->year_size;i++){
        if(data->year[i]==0x00) printf(" ");
        else printf("%c",data->year[i]);
    }
    printf("\n");
    printf("\tCOMPOSER  ----> ");
     for(int i=0;i<data->composer_size;i++){
        if(data->composer[i]==0x00) printf(" ");
        else printf("%c",data->composer[i]);
    }
    printf("\n");
    printf("\tGENRE     ----> ");
     for(int i=0;i<data->genre_size;i++){
        if(data->genre[i]==0x00) printf(" ");
        else printf("%c",data->genre[i]);
    }
    printf("\n");
    printf("\t-----------------------------------------------------\n");
}
/**
TODO: Add documention as sample given
 */
void view_tags(const char *filename) {
    TagData *data = read_id3_tags(filename);
    if (!data) {
        display_error("Failed to read ID3 tags.");
        return;
    }
    display_metadata(data);
    free_tag_data(data);
}
