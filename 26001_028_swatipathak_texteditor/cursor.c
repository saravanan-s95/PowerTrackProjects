#include "../include/header.h"

void cursor_init(Cursor* cursor,line** head,line** tail){
    if(*head==NULL){
        create_empty_line(head,tail,"");
    }
    cursor->currline=*head;
    cursor->lineno=1;
    cursor->col=0;
}
void create_empty_line(line**head,line**tail,char* str){
    line* new=malloc(sizeof(line));
    new->next=NULL;
    new->prev=NULL;
    strcpy(new->str,str);
    *head=*tail=new;
}

void move_cursor_right(Cursor* cursor){
    (cursor->col)++;
    int len=strlen(cursor->currline->str);
    if(cursor->col>len){
        if(cursor->currline->next==NULL){
            cursor->col--;
        }
        else{
            cursor->currline=cursor->currline->next;
            (cursor->lineno)++;
            cursor->col=0;
        }
    }
}
void move_cursor_left(Cursor* cursor){
    (cursor->col)--;
    if(cursor->col==-1){
        if(cursor->currline->prev==NULL){
            (cursor->col)++;
        }
        else{
            cursor->currline=cursor->currline->prev;
            (cursor->col)=strlen(cursor->currline->str);
            (cursor->lineno)--;
        }
    }
}

void move_cursor_up(Cursor* cursor){
    if(cursor->currline->prev==NULL){
        cursor->col=0;
    }
    else{
        cursor->currline=cursor->currline->prev;
        (cursor->lineno)--;
        int len=strlen(cursor->currline->str);
        if(cursor->col>len){
            cursor->col=len;
        }
    }
}
void move_cursor_down(Cursor* cursor){
    if(cursor->currline->next==NULL){
        cursor->col=strlen(cursor->currline->str);
    }
    else{
        cursor->currline=cursor->currline->next;
        (cursor->lineno)++;
        int len=strlen(cursor->currline->str);
        if(cursor->col>len)
            cursor->col=len;
    }
}

