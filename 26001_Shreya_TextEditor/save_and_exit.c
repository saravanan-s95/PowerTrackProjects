#include<stdio.h>
#include"text_editor.h"
#include<stdlib.h>
#include<string.h>
/*******************************************************************************************************************************************************************
*Title			: Save And Exit
*Description	: This function prompts the user to confirm whether changes should be saved before 
                  exiting. If confirmed, it validates the file name, checks for an existing file 
                  with the same name and prompts for overwrite confirmation if needed, 
                  writes the document contents to the file, and then terminates the program.
*Prototype		: void save_and_exit(TextEditor text_Editor, char*filename);
*Input Parameters	: text_Editor: A copy of the TextEditor structure whose contents are to be saved.
			: filename: Pointer to the string containing the file name passed at program startup, 
            or NULL if none was given.
*Output			: None
*******************************************************************************************************************************************************************/
void save_and_exit(TextEditor text_Editor,char*filename){
    char option[2];
        printf("Do you want to save the changes?\n(y/n): ");
        scanf("%s",option);
    
        if(strcmp(option,"y")==0){
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
                        printf("Overwrite the existing file? Enter 'yes' to overwrite, 'no' otherwise\n");
                        scanf("%s",option);
                        getchar();
                        if(strcmp(option,"yes")==0){
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
                        else if(strcmp(option,"no")==0){
                            
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
        exit(0);
    }
    else if(strcmp(option,"n")==0){
        exit(0);
    }
    else{
        printf("Incorrect option");
    }
    
}
