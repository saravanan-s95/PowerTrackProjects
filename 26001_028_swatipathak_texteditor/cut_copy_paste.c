#include "../include/header.h"
void copy(Editor* editor){
    if(editor->selection.active){
        copy_selected_text(editor);
        editor->selection.active=0;
        editor->clipb.text_line=TEXT;
    }
    else{
        copy_line(editor);
        editor->clipb.text_line=LINE;
    }
}
void cut(Editor* editor){
    Action action;
    if(editor->selection.active){
        cut_selected_text(editor);
        editor->selection.active=0;
        editor->clipb.text_line=TEXT;
        action.cursor_col=editor->cursor.col;
        action.cursor_lineno=editor->cursor.lineno;
    }
    else{
        action.cursor_col=editor->cursor.col;
        action.cursor_lineno=editor->cursor.lineno;
        cut_line(editor);
        editor->clipb.text_line=LINE;
        
    }
    editor->modified=1;
    action.clipboard=editor->clipb;
    action.selection=editor->selection;
    action.type=DELETE_SELECTION;
    push(&(editor->undotop),action);
    clear_redo(&(editor->redotop));

}
void paste(Editor* editor){
    Action action;
    action.cursor_lineno=editor->cursor.lineno;
    action.cursor_col=editor->cursor.col;
    if(editor->clipb.text_line==TEXT){
        paste_selected_text(editor);
        action.Lineno=editor->cursor.lineno;
        action.col=editor->cursor.col;
    }
    else if(editor->clipb.text_line==LINE){
        paste_line(editor);
    }
    else{
        return;
    }
    editor->modified=1;
    action.clipboard=editor->clipb;
    action.selection=editor->selection;
    action.type=INSERT_SELECTION;
    push(&(editor->undotop),action);
    clear_redo(&(editor->redotop));
}
void copy_line(Editor* editor){
    editor->clipb.line[0]=malloc(sizeof(char)*1000);
    strcpy(editor->clipb.line[0],editor->cursor.currline->str);
    editor->clipb.line_count=1;
    editor->selection.active=0;
    editor->clipb.text_line=LINE;
}
void paste_line(Editor* editor){
    if(editor->clipb.line_count>0){
        line* new=malloc(sizeof(line));
        strcpy(new->str,editor->clipb.line[0]);
        new->next=editor->cursor.currline;
        new->prev=editor->cursor.currline->prev;
        if(new->prev!=NULL){
            new->prev->next=new;
        }
        new->next->prev=new;
        if(editor->head==editor->cursor.currline){
            editor->head=new;
        }
        // editor->cursor.col=0;
        ((editor->cursor).lineno)++;
    }
}

void cut_line(Editor* editor){
    editor->clipb.line[0]=malloc(sizeof(char)*1000);
    strcpy(editor->clipb.line[0],editor->cursor.currline->str);
    if(editor->cursor.currline==editor->head && editor->head==editor->tail){
        strcpy(editor->clipb.line[0],editor->cursor.currline->str);
        editor->cursor.currline->str[0]='\0';
        editor->cursor.col=0;
        editor->cursor.lineno=1;
        editor->cursor.currline=editor->head;
        // editor->clip.copy=1;
        editor->selection.active=0;
        editor->clipb.line_count=1;
    }
    else if(editor->cursor.currline==editor->tail){
        strcpy(editor->clipb.line[0],editor->cursor.currline->str);
        line* del=editor->cursor.currline;
        del->prev->next=del->next;
        editor->tail=del->prev;
        editor->cursor.col=0;
        (editor->cursor.lineno)--;
        editor->cursor.currline=del->prev;
        // editor->clip.copy=1;
        editor->selection.active=0;
        editor->clipb.line_count=1;
        free(del);
    }
    else{
        strcpy(editor->clipb.line[0],editor->cursor.currline->str);
        line* del=editor->cursor.currline;
        del->next->prev=del->prev;
        if(del->prev!=NULL){
            del->prev->next=del->next;
        }
        if(del==editor->head)
            editor->head=del->next;
        editor->cursor.col=0;
        editor->cursor.currline=del->next;
        // editor->clip.copy=1;
        editor->selection.active=0;
        editor->clipb.line_count=1;
        free(del);
    }
    editor->clipb.text_line=LINE;
}

void copy_selected_text(Editor* editor){
    int lineno=1;
    int start_col=editor->selection.start_col;
    line* curr_line=editor->selection.start_line;
    int start_line=editor->selection.start_lineno;
    int end_col=editor->selection.end_col;
    int end_line=editor->selection.end_lineno;
    if(start_line==end_line){
        editor->clipb.line[0]=malloc(sizeof(char)*1000);
        editor->clipb.line_count=1;
        int len=end_col-start_col;
        strncpy(editor->clipb.line[0],curr_line->str+start_col,len);
        editor->clipb.line[0][len]='\0';
    }
    else{
        editor->clipb.line_count=0;
        while(start_line!=end_line){
            editor->clipb.line[(editor->clipb.line_count)]=malloc(sizeof(char)*1000);
            strcpy(editor->clipb.line[(editor->clipb.line_count)++],curr_line->str+start_col);
            start_col=0;
            start_line++;
            curr_line=curr_line->next;
        }
        editor->clipb.line[(editor->clipb.line_count)]=malloc(sizeof(char)*1000);
        strncpy(editor->clipb.line[(editor->clipb.line_count)],curr_line->str+start_col,end_col);
        editor->clipb.line[(editor->clipb.line_count)++][end_col]='\0';
    }
    // editor->clipb.copy=0;
    editor->clipb.text_line=TEXT;
    for(int i=0;i<editor->clipb.line_count;i++){
        printf("%s\n",editor->clipb.line[i]);
    }
}
void paste_selected_text(Editor* editor){
    int cur_col=editor->cursor.col;
    int cur_lineno=editor->cursor.lineno;
    line* curr_line=editor->cursor.currline;
    char str[1000];
    strcpy(str,curr_line->str+cur_col);
    strcpy(curr_line->str+cur_col,editor->clipb.line[0]);
    for(int i=1;i<editor->clipb.line_count;i++){
        line* new=malloc(sizeof(line));
        strcpy(new->str,editor->clipb.line[i]);
        new->prev=curr_line;
        new->next=curr_line->next;
        curr_line->next=new;
        if(new->next!=NULL){
            new->next->prev=new;
        }
        if(curr_line==editor->tail){
            editor->tail=new;
        }
        curr_line=curr_line->next;
    }
    int len=strlen(curr_line->str);
    strcat(curr_line->str,str);
    editor->cursor.col=len;
    editor->cursor.currline=curr_line;
    editor->cursor.lineno+=editor->clipb.line_count-1;
}

void cut_selected_text(Editor* editor){
    copy_selected_text(editor);
    strcpy(editor->selection.start_line->str+editor->selection.start_col,editor->selection.end_line->str+editor->selection.end_col);
    editor->cursor.currline=editor->selection.start_line;
    editor->cursor.lineno=editor->selection.start_lineno;
    editor->cursor.col=editor->selection.start_col;
    if(editor->selection.start_line!=editor->selection.end_line){
        line* del=editor->selection.start_line->next;
        while(del!=editor->selection.end_line){
            del=del->next;
            free(del->prev);
        }
        editor->cursor.currline->next=del->next;
        if(del->next!=NULL){
            del->next->prev=editor->cursor.currline;
        }
        else{
            editor->tail=editor->cursor.currline;
        }
    }
    // editor->clip.copy=0;
    editor->clipb.text_line=TEXT;
}