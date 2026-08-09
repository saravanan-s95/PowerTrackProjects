#include<stdio.h>
#include<string.h>
#include"text_editor.h"
#include"navigation.h"
/*******************************************************************************************************************************************************************
*Title			: Move Up
*Description	: This function moves the cursor to the previous line, if one exists
                  and resets the cursor position to the start of that line.
*Prototype		: void moveup(TextEditor*text_editor);
*Input Parameters	: text_editor: Pointer to the TextEditor structure whose cursor is to be moved.
*Output			: None
*******************************************************************************************************************************************************************/
void moveup(TextEditor*text_editor){
    if(text_editor->cursorLine>0){
        text_editor->cursorLine--;
        text_editor->cursorPos=0;
        text_editor->cursor=text_editor->cursor->prev;
    }
    
}
/*******************************************************************************************************************************************************************
*Title			: Move Down
*Description	: This function moves the cursor to the next line, if one exists
                  and resets the cursor position to the start of that line.
*Prototype		: void movedown(TextEditor*text_editor);
*Input Parameters	: text_editor: Pointer to the TextEditor structure whose cursor is to be moved.
*Output			: None
*******************************************************************************************************************************************************************/
void movedown(TextEditor*text_editor){
    if(text_editor->cursor->next && text_editor->cursor!=text_editor->tail){
        text_editor->cursorLine++;
        text_editor->cursorPos=0;
        text_editor->cursor=text_editor->cursor->next;
    }
    
}
/*******************************************************************************************************************************************************************
*Title			: Move Left
*Description	: This function moves the cursor one position to the left within the current line, 
                  if it is not already at the start.
*Prototype		: void moveleft(TextEditor*text_editor);
*Input Parameters	: text_editor: Pointer to the TextEditor structure whose cursor is to be moved.
*Output			: None
*******************************************************************************************************************************************************************/
void moveleft(TextEditor*text_editor){
    if(text_editor->cursorPos>0)
    text_editor->cursorPos--;
}
/*******************************************************************************************************************************************************************
*Title			: Move Right
*Description	: This function moves the cursor one position to the right within the current line, 
                  if it is not already at the end.
*Prototype		: void moveright(TextEditor*text_editor);
*Input Parameters	: text_editor: Pointer to the TextEditor structure whose cursor is to be moved.
*Output			: None
*******************************************************************************************************************************************************************/
void moveright(TextEditor*text_editor){
    if(text_editor->cursorPos<strlen(text_editor->cursor->buf))
    text_editor->cursorPos++;
}
/*******************************************************************************************************************************************************************
*Title			: Home
*Description	: This function moves the cursor to the start of the current line.
*Prototype		: void home(TextEditor*text_editor);
*Input Parameters	: text_editor: Pointer to the TextEditor structure whose cursor is to be moved.
*Output			: None
*******************************************************************************************************************************************************************/
void home(TextEditor*text_editor){
    text_editor->cursorPos=0;
}
/*******************************************************************************************************************************************************************
*Title			: End
*Description	: This function moves the cursor to the end of the current line.
*Prototype		: void end(TextEditor*text_editor);
*Input Parameters	: text_editor: Pointer to the TextEditor structure whose cursor is to be moved.
*Output			: None
*******************************************************************************************************************************************************************/
void end(TextEditor*text_editor){
    if(text_editor->cursor)
    text_editor->cursorPos=strlen(text_editor->cursor->buf);
}
/*******************************************************************************************************************************************************************
*Title			: Start Of File
*Description	: This function moves the cursor to the first line of the document, at position zero.
*Prototype		: void start_of_file(TextEditor*text_editor);
*Input Parameters	: text_editor: Pointer to the TextEditor structure whose cursor is to be moved.
*Output			: None
*******************************************************************************************************************************************************************/
void start_of_file(TextEditor*text_editor){
    if(text_editor->head){
        text_editor->cursorLine=1;
        text_editor->cursorPos=0;
        text_editor->cursor=text_editor->head;
    }
}
/*******************************************************************************************************************************************************************
*Title			: End Of File
*Description	: This function moves the cursor to the last line of the document, 
                  at the end of that line's text.
*Prototype		: void end_of_file(TextEditor*text_editor);
*Input Parameters	: text_editor: Pointer to the TextEditor structure whose cursor is to be moved.
*Output			: None
*******************************************************************************************************************************************************************/
void end_of_file(TextEditor*text_editor){
    if(text_editor->head){
        text_editor->cursor=text_editor->tail;
        text_editor->cursorPos=strlen(text_editor->cursor->buf);
        line*temp=text_editor->head;
        int line_count=0;
        while(temp){
            line_count++;
            temp=temp->next;
        }
        text_editor->cursorLine=line_count;
    }
}
/*******************************************************************************************************************************************************************
*Title			: Navigation
*Description	: This function displays the navigation command menu and repeatedly reads a command from the user, 
                  calling the corresponding cursor-movement function until the user chooses to exit navigation.
*Prototype		: void navigation(TextEditor*text_editor);
*Input Parameters	: text_editor: Pointer to the TextEditor structure to navigate.
*Output			: None
*******************************************************************************************************************************************************************/
void navigation(TextEditor*text_editor){
    if(text_editor->cursor==NULL){
        printf("Navigation failed. Empty document\n");
        return;
    }
    while(1){
        printf("\n\n-------------------NAVIGATION COMMANDS-------------------\n");
        printf("1.up - move cursor up\n");
        printf("2.down - move cursor down\n");
        printf("3.left - move cursor left\n");
        printf("4.right - move cursor right\n");
        printf("5.home - move cursor to the start of line\n");
        printf("6.end - move cursor to the end of line\n");
        printf("7.sof(start of file) - move cursor to the start of file\n");
        printf("8.eof(end of file) - move cursor to the end of file\n");
        printf("9.exit - exit the navigation\n\n");
        printf("---------------------------------------------------------\n");
        printf("Enter navigation command: ");

        char command[CMD_LEN];
        scanf("%[^\n]",command);
        getchar();
        if(strcmp(command,"up")==0){
            moveup(text_editor);
        }
        else if(strcmp(command,"down")==0){
            movedown(text_editor);
        }
        else if(strcmp(command,"left")==0){
            moveleft(text_editor);
        }
        else if(strcmp(command,"right")==0){
            moveright(text_editor);
        }
        else if(strcmp(command,"home")==0){
            home(text_editor);
        }
        else if(strcmp(command,"end")==0){
            end(text_editor);
        }
        else if(strcmp(command,"exit")==0){
            printf("\n");
            return;
        }
        else if(strcmp(command,"sof")==0){
            start_of_file(text_editor);
            
        }
        else if(strcmp(command,"eof")==0){
            end_of_file(text_editor);
           
        }
        else printf("Invalid command\n");
    }

    
}
