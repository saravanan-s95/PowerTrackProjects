#include <stdio.h>
#include <string.h>
#include "id3_writer.h"
#include "id3_reader.h"
#include "id3_utils.h"
#include<stdlib.h>
void get_image_uth(FILE *fpr,char *extn,char *img_name,int n,int frame_size)
{
     char output[50]="";
    strcpy(output,img_name);
   // printf("-----> %s",output);
    strcat(output,".");
    strcat(output,extn);
    FILE *fpw;
    if((fpw=fopen(output,"w"))==NULL){
        printf("UNABLE TO OPEN File");
        return;
    }
 
    fseek(fpr,2,SEEK_CUR);
    char ch;
    for(int i=0;i<frame_size-n-5;i++)
    {
       fread(&ch,1,1,fpr);
       fseek(fpr,1,SEEK_CUR);
       fwrite(&ch,1,1,fpw); 
    }  
}
void get_image_iso(FILE *fpr ,char *extn,char *img_name,int n,int frame_size){
    // printf("ext = %s\n", extn);
    // printf("name = %s\n", img_name);
    // printf("mime_len = %d\n", n);
    // printf("frame_size = %d\n", frame_size);
    char output[50]="";
    strcpy(output,img_name);
   // printf("-----> %s",output);
    strcat(output,".");
    strcat(output,extn);
    FILE *fpw;
    if((fpw=fopen(output,"w"))==NULL){
        printf("UNABLE TO OPEN File");
        return;
    }

    fseek(fpr,1,SEEK_CUR);
    int i=0;
    char ch,des[100];
    while((ch=fgetc(fpr))!='\0'){
         des[i++]=ch;
    }
    des[i]='\0';
    printf("\tDescription ----> %s\n",des);
    char img_buffer[frame_size-n-5-i];
    fread(img_buffer,sizeof(char),frame_size-n-5-i,fpr);
    fwrite(img_buffer,sizeof(char),frame_size-n-5-i,fpw);
}
int download_image(char *image_name,char *filename)
{
    
     FILE *fpimg;
    if((fpimg=fopen(filename,"r"))==NULL){
        printf("\tUnable to open file\n");
        return 0;

    }
        if(!check_id3_tags_writer(fpimg))
        {
          printf("ERROR\n");
             return 0;
          }
         fseek(fpimg,2,SEEK_CUR);
          int meta_size = get_size(fpimg);   
    while(ftell(fpimg)<= meta_size)
    {
       
        char frame_tag[5];
       // printf("------> frame\n");
        fread(frame_tag,1,4,fpimg);
     //     printf("------> frame_read\n");
        frame_tag[4]='\0';
        // printf("before getsize\n");
        int frame_size = get_frame_size(fpimg);
        //   printf("after getsize\n"); 
        fseek(fpimg,2,SEEK_CUR); // flag 
         // printf("after seeek\n");
        if(!strcmp(frame_tag,"APIC"))
        { 
             //  printf("heyyyy--->\n");
                unsigned char ch;
                
                fread(&ch,1,1,fpimg);
                fseek(fpimg,0,SEEK_CUR);
                //printf("hey-->1\n");
                char mime[50];
                int idx=0;
                if(ch == 0x01)
                {  
                  //  printf("hey-->2\n");
                    fseek(fpimg,2,SEEK_CUR);//bom
                    char c;
                    while((ch=fgetc(fpimg))!='\0'){
                        mime[idx++]=ch;
                    }
                    mime[idx]='\0';
                  //  printf("-----> %s ",mime);
                    get_image_uth(fpimg,&mime[6],image_name,strlen(mime)+1,frame_size-2);
                     break;
                }
                else if(ch == 0x00)
                {
                  // printf("hey-->2\n");
                    char c;
                    while((ch=fgetc(fpimg))!='\0'){
                        mime[idx++]=ch;
                    }
                    mime[idx]='\0';
                     printf("\tImage type : %s \n",mime);
                     printf("\tImage size : %dkb \n",frame_size/1024);
                    get_image_iso(fpimg, &mime[6], image_name, strlen(mime)+1, frame_size);
                    break;
                }
                else if(ch==0x02)
                {
                    char c;
                    while((ch=fgetc(fpimg))!='\0'){
                        mime[idx++]=ch;
                    }
                    mime[idx]='\0';
                    // printf("-----> %s",mime);
                     get_image_uth(fpimg,&mime[6],image_name,strlen(mime)+1,frame_size);
                   break;
                }
        }       
        else
        {
            fseek(fpimg,frame_size,SEEK_CUR);
        }    
    
   }
      return 1;
}

