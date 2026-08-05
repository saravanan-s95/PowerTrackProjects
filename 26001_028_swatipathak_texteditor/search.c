#include "../include/header.h"

int search(Editor* editor){
    printf("Enter word:");
    scanf("%s",editor->search.search_text);
    int start_col=editor->cursor.col;
    int start_line=editor->cursor.lineno;
    line* curr_line=editor->cursor.currline;
    char* start_add=NULL;
    while(curr_line!=NULL){
        if((start_add=strstr(curr_line->str+start_col,editor->search.search_text))==NULL){
            start_col=0;
            start_line++;
            curr_line=curr_line->next;
        }
        else{
            start_col=start_add-curr_line->str;
            editor->search.last_col=editor->cursor.col=start_col;
            editor->search.last_line=editor->cursor.lineno=start_line;
            editor->search.last_match_line=editor->cursor.currline=curr_line;
            editor->search.active_search=1;
            return SUCCESS;
        }
    }
    curr_line=editor->head;
    start_col=0;
    start_line=1;
    while(curr_line!=editor->cursor.currline){
        if((start_add=strstr(curr_line->str,editor->search.search_text))==NULL){
            start_col=0;
            start_line++;
            curr_line=curr_line->next;
        }
        else{
            start_col=start_add-curr_line->str;
            editor->search.last_col=editor->cursor.col=start_col;
            editor->search.last_line=editor->cursor.lineno=start_line;
            editor->search.last_match_line=editor->cursor.currline=curr_line;
            editor->search.active_search=1;
            return SUCCESS;
        }
    }
    char str[1000];
    strncpy(str,(((editor->cursor).currline)->str),editor->cursor.col);
    str[editor->cursor.col]='\0';
    if((start_add=strstr(str,editor->search.search_text))!=NULL){
        start_col=start_add-str;
        editor->cursor.col=start_col;
        editor->search.last_col=start_col;
        editor->search.last_line=editor->cursor.lineno;
        editor->search.last_match_line=editor->cursor.currline;
        editor->search.active_search=1;
        printf("%d %d\n",editor->cursor.lineno,editor->cursor.col);
        return SUCCESS;
    }
    return NOT_FOUND;
}

int search_next(Editor* editor){
    if(editor->search.active_search==0){
        return NOT_FOUND;
    }
    int start_col=editor->search.last_col+1;
    int start_line=editor->search.last_line;
    line* curr_line=editor->search.last_match_line;
    char* start_add=NULL;
    while(curr_line!=NULL){
        if((start_add=strstr(curr_line->str+start_col,editor->search.search_text))==NULL){
            start_col=0;
            start_line++;
            curr_line=curr_line->next;
        }
        else{
            start_col=start_add-curr_line->str;
            editor->search.last_col=editor->cursor.col=start_col;
            editor->search.last_line=editor->cursor.lineno=start_line;
            editor->search.last_match_line=editor->cursor.currline=curr_line;
            return SUCCESS;
        }
    }
    curr_line=editor->head;
    start_col=0;
    start_line=1;
    while(curr_line!=editor->search.last_match_line){
        if((start_add=strstr(curr_line->str,editor->search.search_text))==NULL){
            start_col=0;
            start_line++;
            curr_line=curr_line->next;
        }
        else{
            start_col=start_add-curr_line->str;
            editor->search.last_col=editor->cursor.col=start_col;
            editor->search.last_line=editor->cursor.lineno=start_line;
            editor->search.last_match_line=editor->cursor.currline=curr_line;
            return SUCCESS;
        }
    }
    char str[1000];
    strncpy(str,(((editor->search).last_match_line)->str),editor->search.last_col);
    str[editor->search.last_col]='\0';
    if((start_add=strstr(str,editor->search.search_text))!=NULL){
        start_col=start_add-str;
        editor->cursor.col=start_col;
        editor->cursor.col=editor->search.last_col=start_col;
        editor->cursor.lineno=editor->search.last_line=editor->search.last_line;
        editor->cursor.currline=editor->search.last_match_line=editor->search.last_match_line;
        return SUCCESS;
    }
    return NOT_FOUND;
}

int replace(Editor* editor){
    // printf("Search:");
    // scanf("%s",editor->search.search_text);
    if(search(editor)==NOT_FOUND){
        return NOT_FOUND;
    }
    else{
        system("cls");
        display(editor);
        int key;
        while((key=getkey())==77){//right arrow means next idx
            search_next(editor);
            system("cls");
            display(editor);
        }
        system("cls");
        display(editor);
        if(key==13){//enter - means replace
            char replace[100];
            printf("Replace:");
            scanf("%s",replace);
            int len1=strlen(editor->search.search_text);
            for(int i=0;i<len1;i++){
                delete_right(editor);
            }
            int len2=strlen(replace);
            for(int i=0;i<len2;i++){
                insert_char(editor,replace[i]);
            }
            editor->search.active_search=0;
            editor->modified=1;
            return SUCCESS;
        }
        else if(key==99){//c-key
            editor->search.active_search=0;
            return NO_OPERATION;    
        }
    }
}