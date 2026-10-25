#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "undo_redo.h"

#define SUCCESS 0
#define FAILURE 1

#define MAX_CHARS 100

/* Doubly Linked List for each line */
typedef struct node
{
    char line[MAX_CHARS];
    struct node* prev;
    struct node* next;
}Node;

/* Text Editor */
typedef struct TextEditor
{
    Node* head; // Pointer to first line
    Node* tail; // Pointer to last line
    Node* current; // Pointer to the current line
    int cursorLine; // Line number of the cursor
    int cursorPos; // Column position of the cursor
}TextEditor;

/* Enum for handling commands */
typedef enum 
{
    INSERT_CURRENT_LINE,
    INSERT_NEWLINE,
    DELETE_CHARACTERS_CURRENT_POS,
    DELETE_LINE,
    CURSOR_NAVIGATION,
    COPY,
    CUT,
    PASTE,
    PRINT,
    UNDO,
    REDO,
    OPEN_FILE,
    SAVE_FILE,
    EXIT,
    UNKNOWN
}CommandType;

/* Print Text Editor Menu */
void printMenu();

/* Handle user commands */
CommandType handleCommands(char* command);

/* Insert given text in newline */
int insertTextnewline(TextEditor* editor,DynamicArrayStack* undoStack,char* str);

/* Insert given text in current line */
int insertText(TextEditor* editor,DynamicArrayStack* undoStack,char* str);

/* Delete no of characters from current cursor position */
int deleteCharacters(TextEditor* editor,DynamicArrayStack* undoStack,int del_chars);

/* Delete characters left from current cursor position */
int deleteLeft(TextEditor* editor,DynamicArrayStack* undoStack,int del_chars);

/* Delete characters right from current cursor position */
int deleteRight(TextEditor* editor,DynamicArrayStack* undoStack,int del_chars);

/* Delete current line */
int deleteLine(TextEditor* editor,DynamicArrayStack* undoStack);

/* Copy function */
int copyText(TextEditor editor);

/* Copy characters left from current cursor position */
int copyLeft(TextEditor editor,int copy_chars);

/* Copy characters right from current cursor position */
int copyRight(TextEditor editor,int copy_chars);

/* Cut function */
int cutText(TextEditor* editor,DynamicArrayStack* undoStack);

/* Paste function */
int pasteText(TextEditor* editor,DynamicArrayStack* undoStack);

/* To print text editor data */
void printTextEditor(TextEditor editor);

#endif