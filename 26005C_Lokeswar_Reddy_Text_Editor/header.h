#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

typedef struct Node{
    char *txt;
    struct Node *prv;
    struct Node *nxt;
} Node;

typedef struct{
    Node *head;
    Node *tail;
    Node *cursor;
    int cline;     // Current line number of the cursor (1-based index)
    int cpos;      // Current position of the cursor in the line (0-based index)
} TxtEdtr;

typedef struct{
    char *opr;
    char *text;
    int cline;
    int cpos;
} Action;

typedef struct{
    Action *actions;
    int size;
    int cap;
} DynArrStk;

void printMenu(void);

void handleCommand(TxtEdtr *editor,DynArrStk *undoStack,DynArrStk *redoStack,char *command);

/* Editor Helper Functions */
void initEditor(TxtEdtr *editor);
void freeEditor(TxtEdtr *editor);

Node *createNode(const char *text);
void appendLine(TxtEdtr *editor, const char *text);
void appendDeletedLine(TxtEdtr *editor, const char *text, int target_line);
void insertText(TxtEdtr *editor, const char *text);
void deleteCharacters(TxtEdtr *editor, int count);
void deleteLine(TxtEdtr *editor);

void copyText(TxtEdtr *editor);
void cutText(TxtEdtr *editor);
void pasteText(TxtEdtr *editor);

int openFile(TxtEdtr *editor, const char *fname);
int saveFile(TxtEdtr *editor, const char *fname);
void closeFile(TxtEdtr *editor);

void moveCursorUp(TxtEdtr *editor);
void moveCursorDown(TxtEdtr *editor);
void moveCursorLeft(TxtEdtr *editor);
void moveCursorRight(TxtEdtr *editor);

void jumpToStartOfFile(TxtEdtr *editor);
void jumpToEndOfFile(TxtEdtr *editor);

void jumpToStartOfLine(TxtEdtr *editor);
void jumpToEndOfLine(TxtEdtr *editor);

void displayText(TxtEdtr *editor);
void showCursorPosition(TxtEdtr *editor);
int findText(TxtEdtr *editor,const char *search);
int replaceText(TxtEdtr *editor,const char *search,const char *replace);


void initStack(DynArrStk *stk);
void pushStack(DynArrStk *stk,const char *opr,const char *text,int line,int pos);
int popStack(DynArrStk *stk, Action *action);
void clearStack(DynArrStk *stk);
void freeStack(DynArrStk *stk);

void undo(TxtEdtr *editor,DynArrStk *undoStk,DynArrStk *redoStk);
void redo(TxtEdtr *editor,DynArrStk *undoStk,DynArrStk *redoStk);


void removeInsertedTextAt(TxtEdtr *editor, int line, int pos, const char *text);
Node *getLineNode(TxtEdtr *editor, int lineNumber);
int setCursor(TxtEdtr *editor, int line, int pos);

#endif
