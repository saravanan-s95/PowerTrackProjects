#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include "editor_internal.h"

typedef struct { char *doc; size_t line,col; } State;
static State undo_stack[100], redo_stack[100];
static size_t undo_n, redo_n;
static void free_state(State *s){free(s->doc);s->doc=NULL;}
static void clear_stack(State *s,size_t *n){while(*n)free_state(&s[--(*n)]);}
static int push(State *s,size_t *n,const char *doc,size_t line,size_t col){
    if(*n==100){free_state(&s[0]);memmove(s,s+1,99*sizeof *s);*n=99;}
    s[*n].doc=strdup(doc?doc:""); if(!s[*n].doc)return -1;
    s[*n].line=line;s[*n].col=col;(*n)++;return 0;
}
void history_clear_redo(void){clear_stack(redo_stack,&redo_n);}
void history_record(TextEditor *e,const char *before,size_t line,size_t col){
    (void)e; push(undo_stack,&undo_n,before,line,col); history_clear_redo();
}
static int apply(TextEditor *e,State *from,size_t *n,State *to,size_t *tn){
    if(!*n)return -1;
    char *current=editor_get_document(e); if(!current)return -1;
    if(push(to,tn,current,e->cursor_line,e->cursor_col)){free(current);return -1;}
    free(current);
    State s=from[--(*n)];
    int rc=editor_set_document(e,s.doc);
    if(!rc){
        for(size_t i=1;i<s.line && e->cursor->next;i++){e->cursor=e->cursor->next;e->cursor_line++;}
        if(s.col>strlen(e->cursor->text))s.col=strlen(e->cursor->text);
        e->cursor_col=s.col;
    }
    free_state(&s);return rc;
}
int editor_undo(TextEditor *e){return apply(e,undo_stack,&undo_n,redo_stack,&redo_n);}
int editor_redo(TextEditor *e){return apply(e,redo_stack,&redo_n,undo_stack,&undo_n);}
void history_destroy(void){clear_stack(undo_stack,&undo_n);clear_stack(redo_stack,&redo_n);}
