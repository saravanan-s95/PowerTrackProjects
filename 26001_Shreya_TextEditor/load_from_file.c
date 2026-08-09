#include<stdio.h>
#include"string.h"
#include<stdlib.h>
#include"text_editor.h"
/*******************************************************************************************************************************************************************
*Title			    : Load From File
*Description	    : This function reads the contents of a specified file, line by line 
                      and loads them into the text editor's document as a doubly linked list of lines, 
                      updating the cursor to point to the last line read.
*Prototype		    : int load_from_file(TextEditor*text_editor, char*filename);
*Input Parameters	: text_editor: Pointer to the TextEditor structure into which the file contents will be loaded.
			        : filename: Pointer to the string containing the name of the file to be read from.
*Output			    : Status (1 on successful load, 0 on failure to open the file or allocate memory)
*******************************************************************************************************************************************************************/
int load_from_file(TextEditor*text_editor,char*filename){
    FILE*fp=fopen(filename,"r");
    if(fp==NULL){
        return 0;
    }
    line temp_line;
    while(fscanf(fp,"%[^\n]\n",temp_line.buf)==1){
        line*newline=malloc(sizeof(line));
        if(newline==NULL){
            return 0;
        } 
        if(text_editor->head==NULL){
            text_editor->head=newline;
            newline->prev=NULL;
        }
        else{
            newline->prev=text_editor->tail;
            text_editor->tail->next=newline;
            
        }
        text_editor->tail=newline;
        text_editor->cursor=newline;
        strcpy(newline->buf,temp_line.buf);
        newline->next=NULL;   
        text_editor->cursorPos=strlen(newline->buf);
        text_editor->cursorLine++;
    }
    
    fclose(fp);
    return 1;
}