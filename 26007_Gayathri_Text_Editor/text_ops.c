#include "text_ops.h"
#include "redo_undo.h"
#include "text_editor.h"
#include "cursor_nav.h"
#include "files_ops.h"


//Function to insert a single character at the current cursor pos
void insertchar(TextEditor *ed, char ch)
{
    //check if editor,cursor is invalid
    if (!ed || !ed->cursor) 
        return;

    //get the curr line where the cursor is
    Node *line = ed->cursor;

    //check if curr line has no text allocation
    if (!line->text) 
    {

        //allocate memory for empty string
        line->text = (char*)malloc(1);

        //add null terminator
        line->text[0] = '\0';
    }

    //create a temp string with character to insert
    char temp[2] = {ch , '\0'};

    //save this insert operation to undo stack
    pushOperation(&ed->undoStack, "insert", temp, ed->cursorLine, ed->cursorPos);

    // clear redo stack
    clearRedo(ed);

    //get the length of the text
    int len = strlen(line->text);

    // realloc - +2 for new char + '\0'
    char *new_text = realloc(line->text, len + 2);

    if (!new_text) 
        return; // malloc fail
    line->text = new_text;

    // shift right
    memmove(&line->text[ed->cursorPos+1], &line->text[ed->cursorPos], len - ed->cursorPos + 1);

    // insert
    line->text[ed->cursorPos] = ch;

    //increase the cursor pos by 1
    ed->cursorPos++;
}

//Function to delete n characters before the cursor
void deletechars(TextEditor*ed, int n)
{
    //check if editor, cusor,text is invalid
    if (!ed || !ed->cursor || !ed->cursor->text) 
        return;

    //get the curr line where the cursor is
    Node *line = ed->cursor;

    //get the length of curr line
    int len = strlen(line->text);

    //if cursor at the start of the line, nothing to delete
    if(ed->cursorPos == 0)
        return;

    //if n is larger than cursor pos
    if(n > ed->cursorPos)
        n = ed->cursorPos;    

    //calculate starting pos where to delete
    int start = ed->cursorPos - n;    
    
    //allocate memory to store the deleted text
    char *deleted = malloc(n+1);

    //copy the character to be deleted into deleted
    strncpy(deleted, &line->text[start], n);

    //add null terminator
    deleted[n] = '\0';

    //save this delete operation in undo stack
    pushOperation(&ed->undoStack, "delete", deleted, ed->cursorLine, start);

    //clear the redo stack
    clearRedo(ed);
    //free the temp memory used for deleted stack
    free(deleted);

    //shift the remaining text left to fill the deleted part
    memmove(&line->text[start], &line->text[ed->cursorPos], len - ed->cursorPos + 1);

    //move the cursor back to start pos
    ed->cursorPos = start;
}


//Function to insert newline in editor
void insertNewLine(TextEditor *ed)
{
    //current line node
    Node *curr = ed->cursor;

    //save ths delete operation
    pushOperation(&ed->undoStack, "newline", "", ed->cursorLine, ed->cursorPos);

    //clear redo
    clearRedo(ed);

    //save remaning part after cursor to new string
    //example "hello world", cursor at 5, rem = "world"(from pos 5 to end)
    char *rem = strdup(&curr->text[ed->cursorPos]);

    //current line at cursor ex hello\0
    curr->text[ed->cursorPos] = '\0';

    //create new node with remaining text world
    Node *nn = createNode(rem);

    //free temp memory we allocated with strdup
    free(rem);

    //insertion
    //newnode next = curr node old nex
    nn->next = curr->next;    

      // newnode prev = curr node
    nn->prev = curr;        

    if(curr->next)
    {
        //prev node point to new node
        curr->next->prev = nn;       
    }

    //curr node next = new node
    curr->next = nn;               

    if(ed->tail == curr)
    {
        //if curr was last node neew become tail
        ed->tail = nn;           
    }

    //move cursor to new node
    ed->cursor = nn;

    //line number increases by 1
    ed->cursorLine++;

    //cursor goes to start new line
    ed->cursorPos = 0;

    //total lines 
    ed->totallines++;
    

}

