#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef enum
{
    SUCCESS = 0,
    FAILURE = -1
} Status_e;

/* Node stores one line of text */
typedef struct Node
{
    char *text;     /* Text is stored in dynamic array */
    int length;  
    struct Node *prev;
    struct Node *next;
}Node;


/* Structure to access the texts, Text storage and cursor state */
typedef struct TextEditor
{
    Node *head;
    Node *tail;
    Node *cursor;

    /* Cursor coordinates (line and column)*/
    int cursorLine;
    int cursorPos;
}TextEditor;

/* Records of Undo/Redo operation, used for reversing operations */
typedef struct Action
{
    char operation;    /* I (insert) or D (Delete) */
    char *text;        /* Affected text */
    int cursorLine;
    int cursorPos;
}Action;

/* Dynamic Stack of Undo/redo actions */
typedef struct DynamicArrayStack
{
    Action *actions;    /* Stack in implemented as dynmaically allocated array */
    int size;
    int capacity;
}DynamicArrayStack;



/* Color codes for different parts of the display */
#define COLOR_RESET   "\033[0m"
#define COLOR_LABEL   "\033[36m"   /* cyan — "Line N:" label */
#define COLOR_TEXT    "\033[37m"   /* white — actual line content */
#define COLOR_LENGTH  "\033[90m"   /* dark gray — (len=N) info */
#define COLOR_CURSOR_LINE_LABEL "\033[33m"   /* yellow — label for the line cursor is on */
#define COLOR_STATUS  "\033[32m"  /* green — cursor position status line */
#define COLOR_HEADER  "\033[34m"  /* blue — box borders */

/* Editor functionalities */
Status_e insertText(TextEditor *editor, int line, int pos, char *newText, DynamicArrayStack *);
Status_e deleteCharacters(TextEditor *editor, int line, int pos, int numChars, DynamicArrayStack *);
Node* getNodeAtLine(TextEditor *editor, int line);

/* Printing editor */
void printLineWithCursor(Node *node, int lineNum, int isCursorLine, int cursorPos);
void printEditor(TextEditor *editor);

/* File Operations */
Status_e openFile(TextEditor *editor, char *filename);
Status_e saveFile(TextEditor *editor, char *filename);
Status_e closeFile(TextEditor *editor);

#endif