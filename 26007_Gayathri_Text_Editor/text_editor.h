#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct  Node
{
    char *text;        //create text node
    struct Node *prev; //prev node
    struct Node *next; //next node

}Node;

typedef struct  Action
{
    char *operation;  // Operation type ("insert" or "delete")
    char *text;       // Text involved in the operation
    int cursorLine;   // Line number where operation occurred
    int cursorPos;    // Column position where operation occurred
} Action;

typedef struct DynamicArrayStack
{
    Action *actions;  // Dynamic array of actions
    int size;         // Current size of the stack
    int capacity;     // Current capacity of the stack
} DynamicArrayStack;

typedef struct TextEditor
{
    Node *head;       // Pointer to the first line
    Node *tail;       // Pointer to the last line
    Node *cursor;     // Pointer to the current line
    int cursorLine;   // Line number of the cursor
    int cursorPos;    // Column position of the cursor
    DynamicArrayStack undoStack;
    DynamicArrayStack redoStack;
    int totallines;

    
} TextEditor;

//create node for editor
TextEditor *createEditor();

//create editor 
Node* createNode(const char *txt);

//free editor
void freeeditor(TextEditor *ed);


#endif