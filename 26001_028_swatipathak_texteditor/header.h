#ifndef HEADER_H
#define HEADER_H
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>

#define EXIT_EDITOR -1
#define NO_OPERATION -2
#define SUCCESS -3
#define NOT_FOUND -4
#define LINE 1
#define TEXT 2
typedef struct node{
    char str[1000];
    struct node* prev;
    struct node* next;
}line;
typedef struct {
    line* currline;
    int lineno;
    int col;
}Cursor;



typedef enum{
    INSERT_CHAR,
    DELETE_CHAR,
    MERGE_LINES,
    SPLIT_LINES,
    INSERT_SELECTION,
    DELETE_SELECTION
}Actiontype;

typedef struct {
    char* line[1000];
    int line_count;
    int copy;
    int text_line;
}Clipboard;
typedef struct
{
    int active;
    line *start_line;
    int start_lineno;
    int start_col;

    line *end_line;
    int end_lineno;
    int end_col;

} Selection;

typedef struct {
    Actiontype type;
    int Lineno;
    int col;
    int cursor_lineno;
    int cursor_col;
    char str[1000];
    Clipboard clipboard;
    Selection selection;
}Action;

typedef struct stack
{
    Action action;
    struct stack *next;
} Stack;

typedef struct
{
    char search_text[100];
    line *last_match_line;
    int last_line;
    int last_col;
    int active_search;
} SearchState;
// typedef struct{
//     char str[1000];
//     int copy;
// }clipboard;


typedef struct{
    char filename[100];
    SearchState search;
    // clipboard clip;
    Clipboard clipb;
    Selection selection;
    line* head;
    line* tail;
    Cursor cursor;
    int modified;
    Stack* undotop;
    Stack* redotop;
}Editor;

void insertatlast(line**head,line**tail,char* str);
int pop(Stack** top,Action* action);
void push(Stack** top,Action action);

void display(Editor* editor);
void create_empty_line(line**head,line**tail,char* str);

void cursor_init(Cursor* cursor,line** head,line**tail);
void move_cursor_left(Cursor* cursor);
void move_cursor_right(Cursor* cursor);
void move_cursor_up(Cursor* cursor);
void move_cursor_down(Cursor* cursor);
int handle_key_press(int key,Editor* editor);
int getkey();
// void editor_loop(Cursor * cursor,line**head,line**tail);
void editor_init(Editor* editor,char* str);
void editor_loop(Editor* editor);
void insert_char(Editor* editor,char ch);
void insert_newline(Editor* editor);
void delete_left(Editor* editor);
void delete_right(Editor* editor);

void undo(Editor* editor);
void redo(Editor* editor);
void clear_redo(Stack** top);
void insert_char_at(Editor* editor,Action action);
void delete_char_at(Editor* editor,Action action);
void split_line(Editor* editor,Action action);
void merge_line(Editor* editor,Action action);
void insert_selection(Editor* editor,Action action);
void delete_selection(Editor* editor,Action action);

int search(Editor* editor);
int search_next(Editor* editor);
int replace(Editor* editor);
void paste(Editor* editor);
void cut(Editor* editor);
void copy(Editor* editor);
void paste_line(Editor* editor);
void copy_line(Editor* editor);
void cut_line(Editor* editor);
void cut_selected_text(Editor* editor);
void paste_selected_text(Editor* editor);
void copy_selected_text(Editor* editor);
void select(Editor* editor);
void save_file(Editor* editor);
int exit_editor(Editor* editor);
#endif