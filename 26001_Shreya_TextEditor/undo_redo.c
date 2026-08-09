#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"undo_redo.h"
/*******************************************************************************************************************************************************************
*Title			: Undo
*Description	: This function reverses the most recent insert or delete operation recorded on the undo stack.
                  For an insert that created a new line, the line is removed; 
                  for an insert that appended or spliced text, the line's previous content is restored. 
                  For a delete that removed a line, the line is recreated at its original position, 
                  for a partial delete, the line's previous content is restored. The reversed operation is pushed onto the redo stack.
*Prototype		: void Undo(TextEditor*text_editor, DynamicArrayStack*undo, DynamicArrayStack*redo);
*Input Parameters	: text_editor: Pointer to the TextEditor structure on which the undo is to be performed.
			: undo: Pointer to the undo stack.
			: redo: Pointer to the redo stack.
*Output			: None
*******************************************************************************************************************************************************************/
void Undo(TextEditor*text_editor, DynamicArrayStack*undo,DynamicArrayStack*redo){
    //pop operation from undo, push to redo
    //empty stack scenario
    if(undo->size==0){
        printf("Undo failed\n");
        return;
    }
    if(redo->size==redo->capacity){
        redo->capacity=redo->capacity*2;
        redo->actions=realloc(redo->actions,redo->capacity*sizeof(Action));
        if(redo->actions==NULL){
            return;
        }
    }
    if(strcmp(undo->actions[undo->size-1].operation,"insert")==0){
        //update texteditor
        if(text_editor->head==NULL){
            printf("undo failed.\n");
            return;
        }
        
        redo->actions[redo->size].cursorLine=undo->actions[undo->size-1].cursorLine;
        redo->actions[redo->size].cursorPos=undo->actions[undo->size-1].cursorPos;
        strcpy(redo->actions[redo->size].new_text,undo->actions[undo->size-1].new_text);
        strcpy(redo->actions[redo->size].old_text,undo->actions[undo->size-1].old_text);
        strcpy(redo->actions[redo->size].operation,undo->actions[undo->size-1].operation);

        if(undo->actions[undo->size-1].created_new_node==1){
            //a new line was created in empty document
            
            redo->actions[redo->size].created_new_node=1;
            if(text_editor->head==text_editor->tail){
                text_editor->cursor->next=NULL;
                text_editor->cursor->prev=NULL;
                free(text_editor->cursor);
                text_editor->cursorLine=0;
                text_editor->cursorPos=0;
                text_editor->head=text_editor->tail=text_editor->cursor=NULL;
                printf("Line deleted successfully\n");
            }
            else{
                
                //if cursor is inbetween and a new line was created
                int line_num=0;
                line*temp1=text_editor->head;
                while(temp1){
                    line_num++;
                    if(undo->actions[undo->size-1].cursorLine==line_num){
                        if(temp1->prev){
                            temp1->prev->next=temp1->next;
                        }
                        
                        else{
                            text_editor->head=temp1->next;
                        }
                        if(temp1->next){
                            temp1->next->prev=temp1->prev;
                        }
                        else{
                            text_editor->tail=temp1->prev;
                        }
                        if(temp1->prev){
                            text_editor->cursor=temp1->prev;
                            text_editor->cursorLine=line_num-1;
                        }
                        else{
                            text_editor->cursor=temp1->next;
                            text_editor->cursorLine=line_num;
                        }
                        text_editor->cursorPos=strlen(text_editor->cursor->buf);
                        //text_editor->cursorPos=0;
                        free(temp1);
                        break;
                    }
                    temp1=temp1->next;
                }
            }
        }
        else{
            redo->actions[redo->size].created_new_node=0;
            int line_num=0;
            line*temp=text_editor->head;
            while(temp){
                line_num++;
                if(undo->actions[undo->size-1].cursorLine==line_num){
                    strcpy(temp->buf,undo->actions[undo->size-1].old_text);
                    text_editor->cursor=temp;
                    text_editor->cursorPos=strlen(text_editor->cursor->buf);
                   // text_editor->cursorPos=0;
                    text_editor->cursorLine=line_num;
                    break;
                }
                temp=temp->next;
            }
            
            
        }
        redo->size++;
        undo->size--;

    }
    else if(strcmp(undo->actions[undo->size-1].operation,"delete")==0){
        redo->actions[redo->size].cursorLine=undo->actions[undo->size-1].cursorLine;
        redo->actions[redo->size].cursorPos=undo->actions[undo->size-1].cursorPos;
        strcpy(redo->actions[redo->size].new_text,undo->actions[undo->size-1].new_text);
        strcpy(redo->actions[redo->size].old_text,undo->actions[undo->size-1].old_text);
        strcpy(redo->actions[redo->size].operation,undo->actions[undo->size-1].operation);
        
        if(undo->actions[undo->size-1].created_new_node==1){
            redo->actions[redo->size].created_new_node=1;
            line*newline=malloc(sizeof(line));
            if(newline==NULL){
                return;
            }
            strcpy(newline->buf,undo->actions[undo->size-1].old_text);
            //find the pos-1 position and insert
            line*temp=text_editor->head;
            if(temp==NULL){
                text_editor->head=text_editor->tail=text_editor->cursor=newline;
                text_editor->cursorLine=1;
                text_editor->cursorPos=strlen(newline->buf);
                newline->next=NULL;
                newline->prev=NULL;
                
            }
            else{
                int line_num=0;
                while(temp){
                    line_num++;
                    if(line_num==undo->actions[undo->size-1].cursorLine-1){
                        newline->next=temp->next;
                        newline->prev=temp;
                        if(temp->next){
                            temp->next->prev=newline;
                        }
                        temp->next=newline;
                        break;
                    }
                    temp=temp->next;
                }
                
                text_editor->cursor=newline;    
                text_editor->cursorLine=++line_num;
                text_editor->cursorPos=strlen(newline->buf);
                
                if(text_editor->cursor->next==NULL){
                    text_editor->tail=text_editor->cursor;
                }
            }
            
            undo->size--;
        }
        else{
            redo->actions[redo->size].created_new_node=0;
            line*temp=text_editor->head;
            
            int line_num=0;
                while(temp){
                    line_num++;
                    if(line_num==undo->actions[undo->size-1].cursorLine){
                        strcpy(temp->buf,undo->actions[undo->size-1].old_text);
                        break;
                    }
                    temp=temp->next;
                }
                text_editor->cursorPos=strlen(temp->buf);
                text_editor->cursorLine=line_num;
                text_editor->cursor=temp;
                undo->size--;
        }
        redo->size++;
    }
}
/*******************************************************************************************************************************************************************
*Title			: Redo
*Description	: This function reapplies the most recently undone operation from the redo stack. 
                  For an insert that created a new line, the line is recreated at its original position,
                  for an insert that appended or spliced text, the line's resulting content is reapplied. 
                  For a delete that removed a line, the line is removed again, 
                  for a partial delete, the line's truncated content is reapplied. 
                  The reapplied operation is pushed back onto the undo stack.
*Prototype		: void Redo(TextEditor*text_editor, DynamicArrayStack*undo, DynamicArrayStack*redo);
*Input Parameters	: text_editor: Pointer to the TextEditor structure on which the redo is to be performed.
			: undo: Pointer to the undo stack.
			: redo: Pointer to the redo stack.
*Output			: None
*******************************************************************************************************************************************************************/
void Redo(TextEditor*text_editor, DynamicArrayStack*undo,DynamicArrayStack*redo){
    if(redo->size==0){
        printf("Redo failed\n");
        return;
    }
    if(undo->size==undo->capacity){
        undo->capacity=undo->capacity*2;
        undo->actions=realloc(undo->actions,undo->capacity*sizeof(Action));
        if(undo->actions==NULL){
            return;
        }
    }
        undo->actions[undo->size].cursorLine=redo->actions[redo->size-1].cursorLine;
        undo->actions[undo->size].cursorPos=redo->actions[redo->size-1].cursorPos;
        strcpy(undo->actions[undo->size].new_text,redo->actions[redo->size-1].new_text);
        strcpy(undo->actions[undo->size].old_text,redo->actions[redo->size-1].old_text);
        strcpy(undo->actions[undo->size].operation,redo->actions[redo->size-1].operation);
            
    if(strcmp(redo->actions[redo->size-1].operation,"insert")==0){
        if(redo->actions[redo->size-1].created_new_node==1){
            undo->actions[undo->size].created_new_node=1;
            line*newline=malloc(sizeof(line));
            if(newline==NULL){
                return;
            }
            strcpy(newline->buf,redo->actions[redo->size-1].new_text);
            //find the pos-1 position and insert
            line*temp=text_editor->head;
            if(temp==NULL){
                text_editor->head=text_editor->tail=text_editor->cursor=newline;
                text_editor->cursorLine=1;
                text_editor->cursorPos=strlen(newline->buf);
                newline->next=NULL;
                newline->prev=NULL;
                
            }
            else{
                int line_num=0;
                while(temp){
                    line_num++;
                    if(line_num==redo->actions[redo->size-1].cursorLine-1){
                        newline->next=temp->next;
                        newline->prev=temp;
                        if(temp->next){
                            temp->next->prev=newline;
                        }
                        temp->next=newline;
                        break;
                    }
                    temp=temp->next;
                }
                text_editor->cursor=newline;
                if(text_editor->cursor->next==NULL){
                    text_editor->tail=text_editor->cursor;
                }
                text_editor->cursorLine=++line_num;//error here?
                text_editor->cursorPos=strlen(newline->buf);
            }
            
            redo->size--;         
            undo->size++;   
        }
        else{
            undo->actions[undo->size].created_new_node=0;
            line*temp=text_editor->head;
            //check about linenum
            int line_num=0;
                while(temp){
                    line_num++;
                    if(line_num==redo->actions[redo->size-1].cursorLine){
                        strcpy(temp->buf,redo->actions[redo->size-1].new_text);
                        break;
                    }
                    temp=temp->next;
                }
                text_editor->cursorPos=strlen(temp->buf);
                redo->size--;
                undo->size++;
        }
    }
    else if(strcmp(redo->actions[redo->size-1].operation,"delete")==0){
        if(redo->actions[redo->size-1].created_new_node==1){
            //this delete had removed an entire line - redo must remove it again
            undo->actions[undo->size].created_new_node=1;
            
            if(text_editor->head==text_editor->tail){
                text_editor->cursor->next=NULL;
                text_editor->cursor->prev=NULL;
                free(text_editor->cursor);
                text_editor->cursorLine=0;
                text_editor->cursorPos=0;
                text_editor->head=text_editor->tail=text_editor->cursor=NULL;
            }
            else{
                //locate the line by number, since cursor may have moved via navigation
                line*temp=text_editor->head;
                int line_num=0;
                while(temp){
                    line_num++;
                    if(line_num==redo->actions[redo->size-1].cursorLine){
                        break;
                    }
                    temp=temp->next;
                }
                //unlink temp
                if(temp->prev){
                    temp->prev->next=temp->next;
                }
                else{
                    text_editor->head=temp->next;
                }
                if(temp->next){
                    temp->next->prev=temp->prev;
                }
                else{
                    text_editor->tail=temp->prev;
                }
                //move cursor to a sensible neighboring line
                if(temp->prev){
                    text_editor->cursor=temp->prev;
                    text_editor->cursorLine=line_num-1;
                }
                else{
                    text_editor->cursor=temp->next;
                    text_editor->cursorLine=line_num;
                }
                //text_editor->cursorPos=strlen(text_editor->cursor->buf);
                text_editor->cursorPos=0;
                free(temp);
            }
            redo->size--;
            undo->size++;
        }
        else{
            //partial delete - redo re-applies the truncated result
            undo->actions[undo->size].created_new_node=0;
            line*temp=text_editor->head;
            int line_num=0;
            while(temp){
                line_num++;
                if(line_num==redo->actions[redo->size-1].cursorLine){
                    strcpy(temp->buf,redo->actions[redo->size-1].new_text);
                    break;
                }
                temp=temp->next;
            }
            text_editor->cursor=temp;
            text_editor->cursorPos=redo->actions[redo->size-1].cursorPos;
            redo->size--;
            undo->size++;
        }
    }
}
