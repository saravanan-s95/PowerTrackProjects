#include<stdio.h>
#include"display.h"
/*******************************************************************************************************************************************************************
*Title			: Display
*Description	: This function prints the current state of the text editor, 
                  showing each line of text along with the cursor marker at its 
                  current position within the document.
*Prototype		: void display(TextEditor text_editor);
*Input Parameters	: text_editor: A copy of the TextEditor structure containing the head of the line list, and the current cursor line and cursor position.
*Output			: prints the document contents and cursor position to standard output
*******************************************************************************************************************************************************************/
void display(TextEditor text_editor){
    if(text_editor.head==NULL){
        printf("Empty document.\n\n");
        return;
    }
    int line_num=0;
    int cursor_pos=0;
    printf("-------------------------------------------------------------\n");
    while(text_editor.head){
        cursor_pos=0;
        line_num++;
        printf("Line:%d ",line_num);
        if(text_editor.cursorPos==0 && line_num==text_editor.cursorLine ){
            printf("|");
        }
        for(int i=0;text_editor.head->buf[i];i++){
            printf("%c",text_editor.head->buf[i]);
            cursor_pos++;
            if(text_editor.cursorPos==cursor_pos && line_num==text_editor.cursorLine){
                printf("|");
            }    
        }
        
        printf("\n");
        text_editor.head=text_editor.head->next;
    }
    printf("\nCursor position: Line %d, Column %d\n",text_editor.cursorLine,text_editor.cursorPos);
    printf("-------------------------------------------------------------\n\n");
}
