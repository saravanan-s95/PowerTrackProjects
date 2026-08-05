#include "../include/header.h"
void editor_init(Editor* editor,char* str){
    editor->head=NULL;
    editor->tail=NULL;
    editor->modified=0;
    editor->undotop=NULL;
    editor->redotop=NULL;
    editor->search.active_search=0;
    editor->selection.active=0;
    editor->clipb.line_count=0;
    strcpy(editor->filename,str);
    char buffer[1000];
    FILE* fp=fopen(editor->filename,"r");
    while(fscanf(fp," %[^\n]",buffer)==1){
        insertatlast(&(editor->head),&(editor->tail),buffer);
    }
    fclose(fp);
    cursor_init(&(editor->cursor),&(editor->head),&(editor->tail));
}
void editor_loop(Editor* editor){
    while(1){
        int key=getkey();
        int status=handle_key_press(key,editor);
        if(status==EXIT_EDITOR)
            return;
    }
}




void save_file(Editor* editor){
    if(editor->modified){
        FILE* fp=fopen(editor->filename,"w");
        line* curr=editor->head;
        while(curr!=NULL){
            fprintf(fp,"%s",curr->str);
            curr=curr->next;
            if(curr!=NULL){
                fprintf(fp,"\n");
            }
        }
        fclose(fp);
        editor->modified=0;
    }
}

int  exit_editor(Editor* editor){
    save_file(editor);
    while(editor->head!=NULL){
        editor->head=editor->head->next;
        free(editor->head->prev);
    }
    return EXIT_EDITOR;
}

