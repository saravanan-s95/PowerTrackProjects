#include<stdio.h>
#include<string.h>
#include"mp3_tag_reader.h"
/***************************************************************************************************************
*Title			: Edit Content
*Description	: Updates a single ID3v2 tag field with a new value.
*Prototype		: int edit_content(char *option, char *new_value, char *filename);
*Input Parameters	: option - field to edit; new_value - replacement text; filename - path to the mp3 file.
*Output			: Writes updated tag to disk. Returns SUCCESS or FAILURE.
****************************************************************************************************************/
int edit_content(char*option,char*new_value,char*filename){
    FILE*fp=fopen(filename,"rb");
    if(fp==NULL){
        printf("Error opening the file\n");
        return FAILURE;
    }

    FILE*tfp=fopen("temp.mp3","wb");
    if(tfp==NULL){
        printf("Error opening the file\n");
        return FAILURE;
    }

    char field[5];
    //map the option and frame id 
    if(strcmp(option,"-t")==0){
        strcpy(field,"TIT2");
    }
    else if(strcmp(option,"-T")==0){
        strcpy(field,"TRCK");
    }
    else if(strcmp(option,"-a")==0){
        strcpy(field,"TPE1");
    }
    else if(strcmp(option,"-A")==0){
        strcpy(field,"TALB");
    }
    else if(strcmp(option,"-y")==0){
        strcpy(field,"TYER");
    }
    
    else if(strcmp(option,"-c")==0){
        strcpy(field,"COMM");
    }
    else if(strcmp(option,"-g")==0){
        strcpy(field,"TCON");
    }
 
    unsigned char header[10];
    fread(header,10,1,fp);
    fwrite(header,10,1,tfp);

    //read size
    unsigned int size=0;
    size=(header[6]<<21)|(header[7]<<14)|(header[8]<<7)|(header[9]);


    int bytes_consumed=0;
    char frame_id[5];
    int old_frame_size=0;
    int found=0;
    while(bytes_consumed<size){
        //copy first 10 bytes
        unsigned char frame[10];
        fread(frame,10,1,fp);
        

        if(frame[0]==0 && frame[1]==0 && frame[2]==0 && frame[3]==0){
            if(found==0){
                printf("Frame not found\n");
            }
            fwrite(frame,10,1,tfp);
            int ch;
            while((ch=fgetc(fp))!=EOF){
                fputc(ch,tfp);
            }
            break;  // hit padding, stop the loop
        }
        //printf("%s\n",frame_id);
        for(int i=0;i<4;i++){
            frame_id[i]=frame[i];
        }
        frame_id[4]='\0';
        old_frame_size = (frame[4]<<24)|(frame[5]<<16)|(frame[6]<<8)|frame[7];//stored in big endian format

        char buffer[old_frame_size+1];
        fread(buffer,old_frame_size,1,fp);
        buffer[old_frame_size]='\0';
        
        int new_frame_size=0;
        if(strcmp(frame_id,field)==0) {
            found=1;
           
            printf("Editing %s...\n", frame_id);
            printf("Old value : %s\n",buffer+1);
            printf("New value : %s\n\n",new_value);
            new_frame_size=strlen(new_value)+1;
            unsigned char size_bytes[4];

            size_bytes[0] =(new_frame_size >> 24) & 0xFF;
            size_bytes[1] =(new_frame_size >> 16) & 0xFF;
            size_bytes[2] =(new_frame_size >> 8) & 0xFF;
            size_bytes[3] =new_frame_size & 0xFF;
            fwrite(frame_id,1,4,tfp);
            fwrite(size_bytes,1,4,tfp);
            fwrite(&frame[8],1,2,tfp);
            fwrite(buffer,1,1,tfp);
            fwrite(new_value,strlen(new_value),1,tfp);

        }
        else{
            
            fwrite(frame,10,1,tfp);
            fwrite(buffer,old_frame_size,1,tfp);
        }
        bytes_consumed+=4+4+2+old_frame_size;
        }
        fclose(tfp);
        fclose(fp);
        remove(filename);
        rename("temp.mp3",filename);
        
        if(found){
            printf("After editing:\n");
            view_content(filename);
        }
        
        return SUCCESS;

}