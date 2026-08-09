#ifndef UNDO_REDO_H
#define UNDO_REDO_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"text_editor.h"

#define STACK_CAPACITY 100
typedef struct{
    char operation[CMD_LEN];
    char old_text[MAX_LEN];
    char new_text[MAX_LEN];
    int cursorLine;
    int cursorPos;
    int deleteCount;
    int created_new_node;
}Action;

typedef struct DynamicArrayStack{
    Action*actions;
    int size;
    int capacity;
}DynamicArrayStack;
void Undo(TextEditor*text_editor, DynamicArrayStack*undo,DynamicArrayStack*redo);
void Redo(TextEditor*text_editor, DynamicArrayStack*undo,DynamicArrayStack*redo);
#endif