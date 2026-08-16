#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS 0
#define FAILURE -1

#define INITIAL_CAPACITY 10

typedef struct Node      //line of text
{
    char *text;

    struct Node *prev;
    struct Node *next;

}Node;                        


typedef struct        //action performed by user to remember for undo/redo actions
{
    char *operation;
    char *text;
    //cursor info
    int cursorLine;
    int cursorPos;

}Action;

typedef struct         //array of actions to store each action 
{
    Action *actions;
    int size;
    int capacity;

}DynamicArrayStack;

typedef struct         //text editor structure
{
    Node *head;          //points to first line
    Node *tail;          //last line
    Node *cursor;        //current line of editing 

    int cursorLine;
    int cursorPos;

    char *filename;      //stores the filename of opened file
    char *clipboard;       //stored the text to cut/paste

    int modified;          //flag to check whether the editor is modified

    DynamicArrayStack undoStack;       //stack to store the action
    DynamicArrayStack redoStack;

}TextEditor;

Node *createNode(const char *text);        //create the lines

void freeAllNodes(TextEditor *editor);         //free all the dynamically created lists 

#endif