#include "../include/header.h"

void undo(Editor* editor){
    Action action;
    int status=pop(&(editor->undotop),&action);
    if(status == NO_OPERATION)
        return;
    if(action.type==INSERT_CHAR){
        delete_char_at(editor,action);
    }
    else if(action.type==DELETE_CHAR){
        insert_char_at(editor,action);
    }
    else if(action.type==MERGE_LINES){
        split_line(editor,action);
    }
    else if(action.type==SPLIT_LINES){
        merge_line(editor,action);
    }
    else if(action.type==INSERT_SELECTION){
        delete_selection(editor,action);
    }
    else if(action.type==DELETE_SELECTION){
        insert_selection(editor,action);
    }
    editor->modified=1;
    push(&(editor->redotop),action);
}

void redo(Editor* editor){
    Action action;
    int status=pop(&(editor->redotop),&action);
    if(status == NO_OPERATION)
        return;
    if(action.type==INSERT_CHAR){
        insert_char_at(editor,action);
    }
    else if(action.type==DELETE_CHAR){
        delete_char_at(editor,action);
    }
    else if(action.type==MERGE_LINES){
        merge_line(editor,action);
    }
    else if (action.type==SPLIT_LINES){
        split_line(editor,action);
    }
    else if(action.type==DELETE_SELECTION){
        delete_selection(editor,action);
    }
    else if(action.type==INSERT_SELECTION){
        insert_selection(editor,action);
    }
    editor->modified=1;
    push(&(editor->undotop),action);
}

void insert_char_at(Editor* editor,Action action){
    line* curr=editor->head;
    int lineno=1;
    while(lineno!=action.Lineno){
        curr=curr->next;
        lineno++;
    }
    int len=strlen(curr->str);
    for(int i=len+1;i>action.col;i--){
        curr->str[i]=curr->str[i-1];
    }
    curr->str[action.col]=(action.str)[0];
    editor->cursor.col=action.cursor_col;
    editor->cursor.lineno=action.cursor_lineno;
    editor->cursor.currline=curr;
    return;
}
void delete_char_at(Editor* editor,Action action){
    line* curr=editor->head;
    int lineno=1;
    while(lineno!=action.Lineno){
        curr=curr->next;
        lineno++;
    }
    int len=strlen(curr->str);
    for(int i=action.col;i<len;i++){
        curr->str[i]=curr->str[i+1];
    }
    editor->cursor.col=action.cursor_col;
    editor->cursor.lineno=action.cursor_lineno;
    editor->cursor.currline=curr;
    return;
}
void merge_line(Editor* editor,Action action){
    printf("m=%d %d",action.Lineno,action.col);
    line* curr=editor->head;
    int lineno=1;
    while(lineno!=action.Lineno){
        curr=curr->next;
        lineno++;
    }
    strcat(curr->str,curr->next->str);
    line* del=curr->next;
    curr->next=del->next;
    if(del->next!=NULL){
        del->next->prev=del->prev;
    }
    if(del==editor->tail){
        editor->tail=curr;
    }
    free(del);
    editor->cursor.col=action.cursor_col;
    editor->cursor.lineno=action.cursor_lineno;
    lineno=1;
    curr=editor->head;
    while(lineno!=action.cursor_lineno){
        curr=curr->next;
        lineno++;
    }
    editor->cursor.currline=curr;
    return;
}

void split_line(Editor* editor,Action action){
    printf("s=%d %d\n",action.Lineno,action.col);
    printf("s=%d %d\n",action.cursor_lineno,action.cursor_col);
    printf("I am stuck1\n");
    line* curr=editor->head;
    int lineno=1;
    while(lineno!=action.Lineno){
        printf("infi");
        curr=curr->next;
        lineno++;
    }
    line* new=malloc(sizeof(line));
    printf("I am stuck2\n");
    strcpy(new->str,curr->str+action.col);
    new->prev=curr;
    new->next=curr->next;
    curr->str[action.col]='\0';
    if(curr->next!=NULL){
        curr->next->prev=new;
    }
    curr->next=new;
    if(curr==editor->tail)
        editor->tail=new;
    editor->cursor.col=action.cursor_col;
    editor->cursor.lineno=action.cursor_lineno;
    lineno=1;
    curr=editor->head;
    printf("I am stuck3\n");
    while(lineno!=action.cursor_lineno){
        printf("I am loop\n");
        curr=curr->next;
        lineno++;
    }
    printf("I am stuck4\n");
    (editor->cursor).currline=curr;
    return;
}

void delete_selection(Editor* editor,Action action){
    if(action.clipboard.text_line==LINE){
        int lineno=1;
        line* curr=editor->head;
        while(lineno!=action.cursor_lineno){
            lineno++;
            curr=curr->next;
        }
        line* del=curr;
        curr->next->prev=curr->prev;
        if(curr->prev!=NULL){
            curr->prev->next=curr->next;
        }
        if(curr==editor->head)
            editor->head=curr->next;
        editor->cursor.col=action.cursor_col;
        editor->cursor.lineno=action.cursor_lineno;
        editor->cursor.currline=curr->next;
        free(curr);
    }
    else{
        int lineno=1;
        char str[1000];
        strcpy(str,action.selection.end_line->str+action.selection.end_col);
        int totalline=action.clipboard.line_count;
        line* curr=editor->head;
        while(lineno!=action.cursor_lineno){
            curr=curr->next;
            lineno++;
        }
        line* lastline=curr;
        while(lineno!=action.Lineno){
            lastline=lastline->next;
            lineno++;
        }
        strcpy(str,lastline->str+action.col);
        curr->str[action.cursor_col]='\0';
        strcat(curr->str,str);
        line* del=curr->next;
        for(int i=1;i<totalline;i++){
            if(del->next==NULL){
                free(del);
                del=NULL;
                editor->tail=curr;
                break;
            }
            else{
                del=del->next;
                free(del->prev);
            }
        }
        if(del!=NULL){
            del->prev=curr;
            curr->next=del;
        }
        editor->cursor.currline=curr;
        editor->cursor.col=action.cursor_col;
        editor->cursor.lineno=action.cursor_lineno;
    }
}

void insert_selection(Editor* editor,Action action){
    if(action.clipboard.text_line==LINE){
        int lineno=1;
        line* curr=editor->head;
        while(lineno!=action.cursor_lineno){
            lineno++;
            curr=curr->next;
        }
        line* new=malloc(sizeof(line));
        strcpy(new->str,action.clipboard.line[0]);
        if(curr==editor->head){
            new->prev=NULL;
            new->next=curr;
            curr->prev=new;
            editor->head=new;
            editor->cursor.col=action.cursor_col;
            editor->cursor.currline=new;
            return;
        }
        else if(curr==NULL){
            new->next=NULL;
            new->prev=editor->tail;
            editor->tail->next=new;
            editor->tail=new;
            editor->cursor.col=action.cursor_col;
            editor->cursor.currline=new;
            editor->cursor.lineno=action.cursor_lineno;
            return;
        }
        else{
            new->next=curr;
            new->prev=curr->prev;
            curr->prev=new;
            new->prev->next=new;
            editor->cursor.col=action.cursor_col;
            editor->cursor.currline=new;
            editor->cursor.lineno=action.cursor_lineno;
            return;
        }
    }
    else{
        char str[1000];
        int lineno=1;
        line* curr=editor->head;
        while(lineno!=action.cursor_lineno){
            lineno++;
            curr=curr->next;
        }
        strcpy(str,curr->str+action.cursor_col);
        int startcol=action.selection.start_col;
        strcpy(curr->str+action.cursor_col,action.clipboard.line[0]);
        for(int i=1;i<action.clipboard.line_count;i++){
            line* new=malloc(sizeof(line));
            strcpy(new->str,action.clipboard.line[i]);
            new->prev=curr;
            new->next=curr->next;
            curr->next=new;
            if(new->next!=NULL)
                new->next->prev=new;
            if(curr==editor->tail)
                editor->tail=new;
            curr=curr->next;
        }
        int len=strlen(curr->str);
        strcat(curr->str,str);
        editor->cursor.col=len;
        editor->cursor.currline=curr;
        editor->cursor.lineno=action.clipboard.line_count+editor->clipb.line_count-1;
            
    }
}