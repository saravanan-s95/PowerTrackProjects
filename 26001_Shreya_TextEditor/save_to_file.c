#include<stdio.h>
#include"text_editor.h"
#include<stdlib.h>
#include<string.h>
/*******************************************************************************************************************************************************************
*Title			: Save To File
*Description	: This function saves the current document to a file. 
                  It uses the file name passed at program startup if available, 
                  otherwise prompts the user for one, validates the file extension, 
                  checks whether the file already exists and prompts for overwrite confirmation 
                  if so and then writes each line of the document to the file.
*Prototype		: void save_to_file(TextEditor text_Editor, char*filename);
*Input Parameters	: text_Editor: A copy of the TextEditor structure whose contents are to be saved.
			: filename: Pointer to the string containing the file name passed at program startup, or NULL if none was given.
*Output			: None
*******************************************************************************************************************************************************************/
void save_to_file(TextEditor text_Editor,char*filename){
    int flag=1;
    char file_name[100]="";
    if(filename!=NULL){
        strcpy(file_name,filename);
    }
    
    char option[4];
    if(file_name[0]=='\0'){
        printf("Enter a file name\n");
        scanf("%s",file_name);
    }
    
    if(file_name[0]!='\0'){
        do{
        char*addr=strrchr(file_name,'.');
        //invalid file name
        if(addr==NULL||strcmp(addr,".txt")!=0){
            printf("Input file is invalid\n");
            printf("Enter valid filename\n");
            scanf("%s",file_name);
        }
        //valid file name
        else{
            FILE*fp=fopen(file_name,"r");
            //if it is a new file
            if(fp==NULL){
                fp=fopen(file_name,"w");
                if(fp==NULL){
                    return;
                }
                while(text_Editor.head){
                    fprintf(fp,"%s\n",text_Editor.head->buf);
                    text_Editor.head=text_Editor.head->next;
                }
                printf("File saved successfully\n");
                flag=0;
                fclose(fp);
            }
            //valid file but it already exists
            else{
                printf("Overwrite the existing file? (y/n): ");
                scanf("%s",option);
                getchar();
                if(strcmp(option,"y")==0){
                    fclose(fp);
                    fp=fopen(file_name,"w");
                    if(fp==NULL){
                        return;
                    }
                    while(text_Editor.head){
                        fprintf(fp,"%s\n",text_Editor.head->buf);
                        text_Editor.head=text_Editor.head->next;
                    }   
                    printf("File saved successfully\n");
                    flag=0;
                    fclose(fp);
                }
                else if(strcmp(option,"n")==0){
                    
                    printf("Enter new file name\n");
                    scanf("%s",file_name);
                    
                }
                else{
                    printf("Invalid input\n");
                }
            }
        }
    }while(flag);
}
}
