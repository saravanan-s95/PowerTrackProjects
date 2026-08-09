
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"text_ops.h"
/*******************************************************************************************************************************************************************
*Title			: Insert Text
*Description	: This function inserts text at the current cursor position. 
                  If no text is given, a new blank line is created after the cursor. 
                  If the document is empty, the first line is created with the given text. 
                  Otherwise, the text is appended or spliced into the current line at the 
                  cursor position. 
                  Each call records the resulting state on the undo stack and clears the redo stack.
*Prototype		: void insert_text(TextEditor*text_editor, char*text, DynamicArrayStack*undo, DynamicArrayStack*redo);
*Input Parameters	: text_editor: Pointer to the TextEditor structure on which the insertion is to be performed.
			: text: Pointer to the string to be inserted, or an empty string to insert a new blank line.
			: undo: Pointer to the undo stack.
			: redo: Pointer to the redo stack.
*Output			: None
*******************************************************************************************************************************************************************/

void insert_text(TextEditor*text_editor,char*text,DynamicArrayStack*undo,DynamicArrayStack*redo){
    //if command is insert
    if(text[0]=='\0'){
        line*newline=malloc(sizeof(line));
        if(newline==NULL){
            return;
        }
        if(undo->size==undo->capacity){
            undo->capacity=undo->capacity*2;
            undo->actions=realloc(undo->actions,undo->capacity*sizeof(Action));
            if(undo->actions==NULL){
                return;
            }
        }
        newline->buf[0]='\0';
        newline->next=NULL;
        if(text_editor->head==NULL){
            text_editor->head=newline; 
            text_editor->tail=newline;
            text_editor->cursor=newline;
            text_editor->cursorLine++;
            text_editor->cursorPos=0;
            newline->prev=NULL;
            newline->next=NULL;
        }
        //insert new line inbetween
        else if(text_editor->cursor->next){
            newline->next=text_editor->cursor->next;
                newline->prev=text_editor->cursor;
            
            text_editor->cursor->next->prev=newline;
            text_editor->cursor->next=newline;
            text_editor->cursor=newline;
            text_editor->cursorLine++;
            text_editor->cursorPos=0;
        }
        //insert newline at last
        else{
            newline->next=NULL;
            newline->prev=text_editor->tail;
            text_editor->tail->next=newline; 
            text_editor->tail=newline;
            text_editor->cursor=newline;
            text_editor->cursorLine++;
            text_editor->cursorPos=0;
        }
        
        //update the stack elements
        redo->size=0;
        undo->actions[undo->size].cursorLine=text_editor->cursorLine;
        undo->actions[undo->size].cursorPos=text_editor->cursorPos;
        strcpy(undo->actions[undo->size].operation,"insert");
        strcpy(undo->actions[undo->size].new_text,text);
        strcpy(undo->actions[undo->size].old_text,"\0");
        undo->actions[undo->size].created_new_node=1;
        undo->size++;
        return;
    }
    //if command is insert <text>
    // //if theres no line, text editor is empty
    if(text_editor->head==NULL){
        //stop if length of new string exceeds max length
        if(strlen(text)+1>MAX_LEN){
            printf("Length of text too long\n");
            return;
        }
        line*newline=(line*)malloc(sizeof(line));
        if(newline==NULL){
            return;
        }
        
        if(undo->size==undo->capacity){
            undo->capacity=undo->capacity*2;
            undo->actions=realloc(undo->actions,undo->capacity*sizeof(Action));
            if(undo->actions==NULL){
                return;
            }
        }
        text_editor->cursorLine++;
        text_editor->cursorPos=strlen(text);
        text_editor->cursor=newline;
        text_editor->head=newline;
        text_editor->tail=newline;
        strcpy(newline->buf,text);
        newline->next=NULL;
        newline->prev=NULL;
        
        redo->size=0;
        undo->actions[undo->size].created_new_node=1;
        undo->actions[undo->size].cursorLine=text_editor->cursorLine;
        undo->actions[undo->size].cursorPos=text_editor->cursorPos;
        strcpy(undo->actions[undo->size].operation,"insert");
        strcpy(undo->actions[undo->size].old_text,"\0");
        strcpy(undo->actions[undo->size].new_text,text);
        undo->size++;      
    }
    //appending to the existing line
    else{
        int len=strlen(text);
        //check if the string can be appended.
        int total_length=strlen(text_editor->cursor->buf)+len+1;

        if((total_length)>MAX_LEN){
            printf("Length of text too long\n");
            return;
        }
        if(undo->size==undo->capacity){
            undo->capacity=undo->capacity*2;
            undo->actions=realloc(undo->actions,undo->capacity*sizeof(Action));
            if(undo->actions==NULL){
                return;
            }
        }
        redo->size=0;
        //if cursor pos is at end of line 
        strcpy(undo->actions[undo->size].old_text,text_editor->cursor->buf);
        undo->actions[undo->size].created_new_node=0;
        if(text_editor->cursorPos==strlen(text_editor->cursor->buf)){
            strcpy((text_editor->cursor->buf)+(text_editor->cursorPos),text);
            text_editor->cursorPos=strlen(text_editor->cursor->buf);
        }
        //cursor inbetween
        else{
            int initial_pos=text_editor->cursorPos;
            char temp_buf[MAX_LEN];
            for(int i=0;i<text_editor->cursorPos;i++){
                temp_buf[i]=text_editor->cursor->buf[i];
            }
            strcpy(temp_buf+text_editor->cursorPos,text);
            strcpy(temp_buf+text_editor->cursorPos+strlen(text),text_editor->cursor->buf+text_editor->cursorPos);
            strcpy(text_editor->cursor->buf,temp_buf);
            text_editor->cursorPos=strlen(text)+initial_pos;//initial pos needed? just take strlen of buf?
        }
        undo->actions[undo->size].cursorLine=text_editor->cursorLine;
        undo->actions[undo->size].cursorPos=text_editor->cursorPos;
        strcpy(undo->actions[undo->size].operation,"insert");
        strcpy(undo->actions[undo->size].new_text,text_editor->cursor->buf);
        undo->size++;
    }
}
/*******************************************************************************************************************************************************************
*Title			: Delete Characters
*Description	: This function deletes a given number of characters from the current cursor position 
                  onward in the current line. 
                  If the number of characters to delete covers the entire remaining line from its start, 
                  the line itself is deleted instead. 
                  The resulting state is recorded on the undo stack and the redo stack is cleared.
*Prototype		: void delete_chars(TextEditor*text_editor, char*str_num, DynamicArrayStack*undo, DynamicArrayStack*redo);
*Input Parameters	: text_editor: Pointer to the TextEditor structure on which the deletion is to be performed.
			: str_num: Pointer to the string containing the number of characters to delete.
			: undo: Pointer to the undo stack.
			: redo: Pointer to the redo stack.
*Output			: None
*******************************************************************************************************************************************************************/
void delete_chars(TextEditor*text_editor,char*str_num,DynamicArrayStack*undo,DynamicArrayStack*redo){

    int num=atoi(str_num);

    //calculate total characters

    if((strlen(text_editor->cursor->buf)- text_editor->cursorPos)<num){
        printf("Cannot delete characters. Only %ld characters are available from current position\n",(strlen(text_editor->cursor->buf)-text_editor->cursorPos));
        return;
    }
    //delete line if number of chars equals length of string
    if(text_editor->cursorPos==0 &&(strlen(text_editor->cursor->buf)- text_editor->cursorPos)==num){
        delete_line(text_editor,undo,redo);
        return;
    }
    if(undo->size==undo->capacity){
        undo->capacity=undo->capacity*2;
        undo->actions=realloc(undo->actions,undo->capacity*sizeof(Action));
        if(undo->actions==NULL){
            return;
        }
    }
    redo->size=0;
    undo->actions[undo->size].created_new_node=0;
    strcpy(undo->actions[undo->size].old_text,text_editor->cursor->buf);    
    strcpy(text_editor->cursor->buf+text_editor->cursorPos,text_editor->cursor->buf+text_editor->cursorPos+num);
    strcpy(undo->actions[undo->size].new_text,text_editor->cursor->buf);
    strcpy(undo->actions[undo->size].operation,"delete");
    undo->actions[undo->size].cursorPos=text_editor->cursorPos;
    undo->actions[undo->size].cursorLine=text_editor->cursorLine;
    undo->size++;
}
/*******************************************************************************************************************************************************************
*Title			: Delete Line
*Description	: This function deletes the entire line at the current cursor position, 
                  unlinking it from the document's line list and moving the cursor to a neighbouring 
                  line if one exists. 
                  The deleted line's contents are recorded on the undo stack and the redo stack is cleared.
*Prototype		: void delete_line(TextEditor*text_editor, DynamicArrayStack*undo, DynamicArrayStack*redo);
*Input Parameters	: text_editor: Pointer to the TextEditor structure on which the deletion is to be performed.
			: undo: Pointer to the undo stack.
			: redo: Pointer to the redo stack.
*Output			: None
*******************************************************************************************************************************************************************/
void delete_line(TextEditor*text_editor,DynamicArrayStack*undo,DynamicArrayStack*redo){
    if(text_editor->head==NULL){
        printf("Empty document\n");
        return;
    }
    if(undo->size==undo->capacity){
        undo->capacity=undo->capacity*2;
        undo->actions=realloc(undo->actions,undo->capacity*sizeof(Action));
        if(undo->actions==NULL){
            return;
        }
    }
    undo->actions[undo->size].created_new_node=1;
    redo->size=0;
    strcpy(undo->actions[undo->size].new_text,"\0");
    strcpy(undo->actions[undo->size].operation,"delete");
    

    strcpy(undo->actions[undo->size].old_text,text_editor->cursor->buf); 
    undo->actions[undo->size].cursorPos=text_editor->cursorPos;
    undo->actions[undo->size].cursorLine=text_editor->cursorLine;
    //one node exists and is deleted  
    if(text_editor->head==text_editor->tail){
        text_editor->cursor->next=NULL;
        text_editor->cursor->prev=NULL;
        free(text_editor->cursor);
        text_editor->head=NULL;
        text_editor->tail=NULL;
        text_editor->cursorPos=0;
        text_editor->cursorLine=0;
        text_editor->cursor=NULL;
    }
    else{
        text_editor->cursorPos=0;
        //if cursor is inbetween and a new line was created
        line*temp=NULL;
        if(text_editor->cursor->prev){
            text_editor->cursor->prev->next=text_editor->cursor->next;
            temp=text_editor->cursor->next;
        }
        else{
            text_editor->head=text_editor->cursor->next;
            temp=text_editor->head;
        }
        if(text_editor->cursor->next){
            text_editor->cursor->next->prev=text_editor->cursor->prev;
        }
        else{
            text_editor->tail=text_editor->cursor->prev;
            temp=text_editor->tail;
            text_editor->cursorLine--;
        }
        free(text_editor->cursor);
        text_editor->cursor=temp;
    }
    printf("Line deleted successfully\n");
    undo->size++;
}
