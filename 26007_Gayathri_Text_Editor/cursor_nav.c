#include "cursor_nav.h"

//function to move cursor one line up
void moveUp(TextEditor *ed)
{
    //check previous line is above the curr line
    if(ed->cursor->prev)
    {
        //move cursor to prev line
        ed->cursor = ed->cursor->prev;

        //decrease the current line number by 1
        ed->cursorLine--;

        //if cursor pos is beyond the length of new line
        //move it to the end of the line
        if(ed->cursorPos > (int)strlen(ed->cursor->text))
            ed->cursorPos = strlen(ed->cursor->text);
    }
}

//function to move cursor one line down
void moveDown(TextEditor *ed)
{
    //check previous line is below the curr line
    if(ed->cursor->next)
    {
        //move cursor to next line
        ed->cursor = ed->cursor->next;

        //increase the current line number by 1
        ed->cursorLine++;

        //if cursor pos is beyond the length of new line
        //move it to the end of the line
        if(ed->cursorPos > (int)strlen(ed->cursor->text))
            ed->cursorPos = strlen(ed->cursor->text);
    }
}


//function to move cursor one position to left
void moveLeft(TextEditor *ed)
{
    //check if cursor is not at the start of curr line
    if(ed->cursorPos > 0)
    {
        //move cursor one character to left
        ed->cursorLine--;
        
    }

    //cursor is at the start of line,check there is a prev line
    else if(ed->cursor->prev)
    {
        //move cursor pointer to previous line
        ed->cursor = ed->cursor->prev;

        //decrease the curr line by 1
        ed->cursorLine--;

        //move cursor to the end of the prev line
        ed->cursorPos = strlen(ed->cursor->text);
    }
}


//function to move cursor one position to right
void moveRight(TextEditor *ed)
{

    //get the length of first line
    int len = strlen(ed->cursor->text);

    //check if cursor is not start the end of curr line
    if(ed->cursorPos < len)
    {
        //move cursor one char to another char
        ed->cursorPos++;
    }

    //if cursor is at the end of the line, check if there is a next line
    else if(ed->cursor->next)
    {
        //move cursor pointer to next line
        ed->cursor = ed->cursor->next;

        //increase the curr line number by 1
        ed->cursorLine++;

        //move cursor to the start of next line
        ed->cursorPos = 0;  
    }
}


