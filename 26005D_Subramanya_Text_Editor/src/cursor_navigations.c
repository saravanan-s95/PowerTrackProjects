#include "cursor_navigations.h"

/* Two more functions yet to implement, move cursor to starting char and move cursor to ending char */

Status_e moveCursorRight(TextEditor *editor)
{
    if(editor->cursorPos < editor->cursor->length)      /* Cursor is before EOL */
    {
        editor->cursorPos++;
    }
    else if(editor->cursor->next != NULL)               /* Cursor at EOL and can go to next line */
    {
        /* Move curor to next line and update line info */
        editor->cursor = editor->cursor->next;
        editor->cursorLine++;

        editor->cursorPos = 0;      /* Cursor starts at 0 pos at new line */
    }
    else
    {
        printf("Cursor has reached EOF!\n");
        return FAILURE;
    }

    return SUCCESS;
}

Status_e moveCursorLeft(TextEditor *editor)
{
    if(editor->cursorPos > 0)      /* Cursor is after SOL */
    {
        editor->cursorPos--;
    }
    else if(editor->cursor->prev != NULL)               /* Cursor at SOL and can go to prev line */
    {
        /* Move curor to prev line and update line info */
        editor->cursor = editor->cursor->prev;
        editor->cursorLine--;

        editor->cursorPos = editor->cursor->length;      /* Cursor starts EOL pos of prev line */
    }
    else
    {
        printf("Cursor has reached SOF!\n");
        return FAILURE;
    }

    return SUCCESS;
}

Status_e moveCursorDown(TextEditor *editor)
{
    /* Clamping of cursorPos to the shorter line length */
    if(editor->cursor->next != NULL)
    {
        /* Move cursor down, increment line and set cursorPos to min */
        editor->cursor = editor->cursor->next;
        editor->cursorLine++;
        editor->cursorPos = min(editor->cursorPos, editor->cursor->length);
    }
    else
    {
        printf("Cursor has reached the Last Line\n");
        return FAILURE;
    }

    return SUCCESS;

}

Status_e moveCursorUp(TextEditor *editor)
{
    /* Clamping of cursorPos to the shorter line length */
    if(editor->cursor->prev != NULL)
    {
        /* Move cursor up, decrement line and set cursorPos to min */
        editor->cursor = editor->cursor->prev;
        editor->cursorLine--;
        editor->cursorPos = min(editor->cursorPos, editor->cursor->length);
    }
    else
    {
        printf("Cursor has reached the First Line\n");
        return FAILURE;
    }

    return SUCCESS;

}

Status_e moveCursorStart(TextEditor *editor)
{
    /* Jump cursor to column 0 of the current line */
    editor->cursorPos = 0;

    return SUCCESS;
}

Status_e moveCursorEnd(TextEditor *editor)
{
    /* Jump cursor to the last column of the current line */
    editor->cursorPos = editor->cursor->length;

    return SUCCESS;
}

Status_e moveCursorToLine(TextEditor *editor, int targetLine)
{
    /* Reuse getNodeAtLine to resolve the target line number to a Node */
    Node *node = getNodeAtLine(editor, targetLine);
    if(node == NULL)
    {
        fprintf(stderr, "ERROR: Invalid line number.\n");
        return FAILURE;
    }

    /* Jump cursor to that node, clamp pos to the new line's length */
    editor->cursor = node;
    editor->cursorLine = targetLine;
    editor->cursorPos = min(editor->cursorPos, node->length);

    return SUCCESS;
}