#include "../include/header.h"

void display(Editor* editor){
    int total_lines=0;
    line* head=editor->head;
    Cursor cursor=editor->cursor;
    printf("--------------------------------------------------------------------------------------\n");
    printf("--------------------------------------Text Editor-------------------------------------\n\n");
    while(head!=NULL){
        total_lines++;
        int len=strlen(head->str);
        if(len==0 && cursor.currline==head){
            printf("|\n");
        }
        // else if(cursor.currline==head && cursor.col==len){
        //     printf("%s|\n",head->str);
        // }
        else{
            for(int i=0;i<len;i++){
                if(cursor.currline==head && cursor.col==i)
                    printf("|");
                if(editor->selection.active){
                    if(editor->selection.start_line==head && editor->selection.start_col==i){
                        printf("<");
                    }
                    else if(editor->selection.end_line==head && editor->selection.end_col==i){
                        printf(">");
                    }
                }
                printf("%c",head->str[i]);
            }
            if(cursor.currline==head && cursor.col==len)
                printf("|");
            if(editor->selection.active){
                if(editor->selection.end_line==head && editor->selection.end_col==len)
                    printf(">");
            }
            printf("\n");
        }
        head=head->next;
    }
    printf("\n--------------------------------------------------------------------------------------\n");
    printf("File: %s  %s\n",editor->filename,editor->modified?"NOT SAVED":"SAVED");
    printf("Line: %d  Col: %d  Lines: %d\n",editor->cursor.lineno,editor->cursor.col,total_lines);
    printf("Selection: %s\n",editor->selection.active?"ON":"OFF");
    printf("Modified: %s\n",editor->modified?"Yes":"No");
    printf("--------------------------------------------------------------------------------------\n");
    return ;
}