#include "../include/header.h"

void select(Editor* editor){
    editor->selection.active=0;
    editor->selection.start_col=editor->cursor.col;
    editor->selection.start_line=editor->cursor.currline;
    editor->selection.start_lineno=editor->cursor.lineno;
    int key;
    while((key=getkey())){
        if(key==77){
            move_cursor_right(&(editor->cursor));
            system("cls");
            display(editor);
        }
        else if(key==75){
            move_cursor_left(&(editor->cursor));
            system("cls");
            display(editor);
        }
        else if(key==72){
            move_cursor_up(&(editor->cursor));
            system("cls");
            display(editor);
        }
        else if(key==80){
            move_cursor_down(&(editor->cursor));
            system("cls");
            display(editor);
        }
        else if(key==20){
            editor->selection.end_lineno=editor->cursor.lineno;
            editor->selection.end_col=editor->cursor.col;
            editor->selection.end_line=editor->cursor.currline;
            editor->selection.active=1;
            if(editor->selection.start_lineno>editor->selection.end_lineno || 
                (editor->selection.end_lineno==editor->selection.start_lineno && editor->selection.start_col>editor->selection.end_col))
                {
                    int startline=editor->selection.start_lineno;
                    int startcol=editor->selection.start_col;
                    line* startl=editor->selection.start_line;
                    editor->selection.start_lineno=editor->selection.end_lineno;
                    editor->selection.start_col=editor->selection.end_col;
                    editor->selection.start_line=editor->selection.end_line;
                    editor->selection.end_col=startcol;
                    editor->selection.end_line=startl;
                    editor->selection.end_lineno=startline;
            }
            break;
        }
        else {
            editor->selection.active=0;
            // return;
            break;
        }
    }
}