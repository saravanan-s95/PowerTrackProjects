#include<stdio.h>
#include<string.h>
/******************************************************************************************
*Title			: View Content
*Description	: Reads and prints all ID3 tag fields from the given file.
*Prototype		: void view_content(char *filename);
*Input Parameters	: filename - path to the mp3 file.
*Output			: Prints tag fields (Title, Artist, Album, Year, Track, Genre, Comment).
*******************************************************************************************/
void view_content(char*filename){
    FILE*fp=fopen(filename,"rb");
    if(fp==NULL){
        printf("Error opening the file\n");
        return;
    }
    char buffer[4];
    fread(buffer,3,sizeof(char),fp);
    buffer[3]='\0';

    unsigned char version_major=0,version_minor=0;
    fread(&version_major,1,1,fp);
    fread(&version_minor,1,1,fp);
    fseek(fp,1,SEEK_CUR);

    printf("ID3 v2.%d:\n",version_major);
    
    if(strcmp(buffer,"ID3")==0){
        unsigned int size=0;
        unsigned char b0=0,b1=0,b2=0,b3=0;
        fread(&b0,sizeof(char),1,fp);
        fread(&b1,sizeof(char),1,fp);
        fread(&b2,sizeof(char),1,fp);
        fread(&b3,sizeof(char),1,fp);
        size=b0<<21|b1<<14|b2<<7|b3;
        int bytes_consumed=0;
        char frame_id[5];
        int frame_size=0;
        
        
        unsigned char s0=0,s1=0,s2=0,s3=0;
        printf("----------------MP3 TAG DETAILS----------------\n");
        while(bytes_consumed<size){
            fread(frame_id,4,1,fp);
            frame_id[4]='\0';
            if(frame_id[0]==0 && frame_id[1]==0 && frame_id[2]==0 && frame_id[3]==0){
                break;  // hit padding, stop the loop
            }
            

            fread(&s0,1,1,fp);
            fread(&s1,1,1,fp);
            fread(&s2,1,1,fp);
            fread(&s3,1,1,fp);
            frame_size = (s0<<24)|(s1<<16)|(s2<<8)|s3;//stored in big endian format

            fseek(fp,2,SEEK_CUR);
            bytes_consumed+=4+4+2+frame_size;
            char buffer[frame_size+1];
            fread(buffer,frame_size,1,fp);
            buffer[frame_size]='\0';
            

            if(strcmp(frame_id,"TIT2")==0) {
                printf("Title: %s\n", buffer+1);
            }
            else if(strcmp(frame_id,"TPE1")==0) {
                printf("Artist: %s\n", buffer+1);
            }
            else if(strcmp(frame_id,"TALB")==0) {
                printf("Album: %s\n", buffer+1);
            }
            else if(strcmp(frame_id,"TYER")==0) {
                printf("Year: %s\n", buffer+1);
            }
            else if(strcmp(frame_id,"TRCK")==0) {
                printf("Track: %s\n", buffer+1);
            }
            else if(strcmp(frame_id,"TCON")==0) {
                printf("Genre: %s\n", buffer+1);
            }
            else if(strcmp(frame_id,"COMM")==0) {
                //skip encoding-1 byte and language-3 bytes
                char *desc=buffer+4;
                //skip description
                char *text=desc+strlen(desc)+1;
                printf("Comment: %s\n",text);
            }
            
         }
         printf("-----------------------------------------------\n");
    }
    //check for version1
    else{
        fseek(fp,-128,SEEK_END);
        fread(buffer,3,sizeof(char),fp);
        buffer[3]='\0';
        if(strcmp(buffer,"TAG")==0){
            //printf("Version1");
            char buf[31];

            fread(buf,30,1,fp);
            buf[30]='\0';
            printf("Title: %s\n",buf);

            fread(buf,30,1,fp);
            buf[30]='\0';
            printf("Artist: %s\n",buf);

            fread(buf,30,1,fp);
            buf[30]='\0';
            printf("Album: %s\n",buf);

            fread(buf,4,1,fp);
            buf[4]='\0';
            printf("Year: %s\n",buf);

            fseek(fp,28,SEEK_CUR);
            
            if(fgetc(fp)==0){
                fseek(fp,-29,SEEK_CUR);
                fread(buf,28,1,fp);
                buf[28]='\0';
                printf("Comment: %s\n",buf);
                //skip zero byte
                fseek(fp,1,SEEK_CUR);

                unsigned char track;
                fread(&track,1,1,fp);
                printf("Track: %hhu\n",track);
            }
            else{
                fseek(fp,-29,SEEK_CUR);
                fread(buf,30,1,fp);
                buf[30]='\0';
                printf("Comment: %s\n",buf);
            }
            unsigned char gen;
            fread(&gen,1,1,fp);
            printf("Genre: %hhu\n",gen);
        }
        else printf("Error: Tag not found");
    }
    fclose(fp);
}