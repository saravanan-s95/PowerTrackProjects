#include"mp3_tag_reader.h"
#include<stdio.h>
#include<string.h>
/****************************************************************************
*Title			: Check Version
*Description	: Checks if the file has an ID3v1 or ID3v2 tag.
*Prototype		: int check_version(char *filename);
*Input Parameters	: filename - path to the mp3 file.
*Output			: Returns SUCCESS if a tag is found, FAILURE otherwise.
****************************************************************************/
int check_version(char*filename){
    FILE*fp=fopen(filename,"rb");
    if(fp==NULL){
        printf("Error opening the file\n");
        return FAILURE;
    }
    char buffer[4];
    fread(buffer,3,sizeof(char),fp);
    buffer[3]='\0';
    if(strcmp(buffer,"ID3")==0){
        fclose(fp);
        return SUCCESS;
    }
    //check for version1
    else{
        fseek(fp,-128,SEEK_END);
        fread(buffer,3,sizeof(char),fp);
        if(strcmp(buffer,"TAG")==0){
            fclose(fp);
            return SUCCESS;
        }
        else printf("Error: Tag not found");
    }
    fclose(fp);
    return FAILURE;
}