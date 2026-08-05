#include "../include/header.h"

void insert_char(Editor* editor,char ch){
    Cursor* cursor=&(editor->cursor);
    int len=strlen(cursor->currline->str);
    for(int i=len;i>=cursor->col;i--){
        cursor->currline->str[i+1]=cursor->currline->str[i];
    }
    cursor->currline->str[cursor->col]=ch;
    if(cursor->col==len){
        cursor->currline->str[len+1]='\0';
    }
    
    Action action;
    action.col=cursor->col;
    action.Lineno=cursor->lineno;
    action.cursor_col=cursor->col;
    action.cursor_lineno=cursor->lineno;
    printf("%d %d\n",action.col,action.Lineno);
    action.str[0]=ch;
    action.str[1]='\0';
    action.type=INSERT_CHAR;
    push(&(editor->undotop),action);
    clear_redo(&(editor->redotop));
    editor->modified=1;
    (cursor->col)++;
}

void insert_newline(Editor* editor){
    Cursor* cursor=&(editor->cursor); 
    char str[1000];int idx=0;
    int len=strlen(cursor->currline->str);
    strcpy(str,cursor->currline->str+(cursor->col));
    cursor->currline->str[cursor->col]='\0';
    line* new=malloc(sizeof(line));
    strcpy(new->str,str);
    new->prev=cursor->currline;
    new->next=cursor->currline->next;
    if(cursor->currline->next!=NULL){
        cursor->currline->next->prev=new;
    }
    cursor->currline->next=new;
    if(cursor->currline==editor->tail){
        editor->tail=new;
    }
    cursor->currline=new;
    Action action;
    action.col=cursor->col;
    action.Lineno=cursor->lineno;
    action.cursor_col=cursor->col;
    action.cursor_lineno=cursor->lineno;
    action.type=SPLIT_LINES;
    strcpy(action.str,str);
    printf("%d %d",action.Lineno,action.col);
    push(&(editor->undotop),action);
    clear_redo(&(editor->redotop));
    (cursor->lineno)++;
    cursor->col=0;
    editor->modified=1;
    return;
}

void delete_left(Editor* editor){
    Cursor* cursor=&(editor->cursor);
    if(cursor->col==0 && cursor->currline==editor->head){
        return;
    }
    else if(cursor->col==0){
        cursor->col=strlen(cursor->currline->prev->str);
        strcat(cursor->currline->prev->str,cursor->currline->str);
        line* del=cursor->currline;
        del->prev->next=del->next;
        if(del->next!=NULL){
            del->next->prev=del->prev;
        }
        cursor->currline=cursor->currline->prev;
        if(editor->tail==del){
            editor->tail=del->prev;
        }
        (cursor->lineno)--;
        editor->modified=1;
        Action action;
        action.col=cursor->col;
        action.Lineno=cursor->lineno;
        action.cursor_col=0;
        action.cursor_lineno=cursor->lineno+1;
        action.type=MERGE_LINES;
        printf("%d %d\n",action.Lineno,action.col);
        strcpy(action.str,del->str);
        push(&(editor->undotop),action);
        clear_redo(&(editor->redotop));
        free(del);    
        return;
    }
    else{
        int len=strlen(cursor->currline->str);
        char del=cursor->currline->str[cursor->col-1];
        for(int i=cursor->col-1;i<len;i++){
            cursor->currline->str[i]=cursor->currline->str[i+1];
        }
        Action action;
        action.col=cursor->col-1;
        action.cursor_col=cursor->col;
        action.cursor_lineno=cursor->lineno;
        action.Lineno=cursor->lineno;
        printf("%d %d\n",action.col,action.Lineno);
        action.type=DELETE_CHAR;
        action.str[0]=del;
        action.str[1]='\0';
        push(&(editor->undotop),action);
        clear_redo(&(editor->redotop));
        (cursor->col)--;
        editor->modified=1;
        return;
    }
    
}

void delete_right(Editor* editor){
    Cursor* cursor=&(editor->cursor);
    int len=strlen(cursor->currline->str);
    if(cursor->currline->next==NULL && cursor->col==len){
        return;
    }
    else if(cursor->col==len){
        line* del=cursor->currline->next;
        strcat(cursor->currline->str,del->str);
        cursor->currline->next=del->next;
        if(del->next!=NULL){
            del->next->prev=del->prev;
        }
        if(del==editor->tail){
            editor->tail=cursor->currline;
        }
        Action action;
        action.col=len;
        action.Lineno=cursor->lineno;
        action.cursor_col=len;
        action.cursor_lineno=cursor->lineno;
        action.type=MERGE_LINES;
        printf("%d %d\n",action.Lineno,action.col);
        strcpy(action.str,del->str);
        push(&(editor->undotop),action);
        clear_redo(&(editor->redotop));
        free(del);
        editor->modified=1;
        return;
    }
    else{
        char del=cursor->currline->str[cursor->col];
        for(int i=cursor->col;i<len;i++){
            cursor->currline->str[i]=cursor->currline->str[i+1];
        }
        Action action;
        action.col=cursor->col;
        action.Lineno=cursor->lineno;
        action.cursor_col=cursor->col;
        action.cursor_lineno=cursor->lineno;
        printf("%d %d\n",action.Lineno,action.col);
        action.type=DELETE_CHAR;
        action.str[0]=del;
        action.str[1]='\0';
        push(&(editor->undotop),action);
        clear_redo(&(editor->redotop));
        editor->modified=1;
        return;
    }
}

void clear_redo(Stack** top){
    while((*top)!=NULL){
        Stack * del=*top;
        *top=(*top)->next;
        free(del);
    }
    *top=NULL;
}