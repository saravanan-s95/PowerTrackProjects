#include "text_editor.h"
#include "text_ops.h"

/* Clipboard buffer — holds whatever was last copied/cut, shared across copy/cut/paste */
static char *clipboard = NULL;

/* Copies substring [startPos, endPos) of the CURRENT line into clipboard */
Status_e copyText(TextEditor *editor, int startPos, int endPos)
{
    Node *node = editor->cursor;

    /* Validate the copy range */
    if((startPos < 0) | (endPos > node->length) | (startPos >= endPos))
    {
        fprintf(stderr, "ERROR: Invalid copy range! Please specify range between [0, %d)\n", node->length);
        return FAILURE;
    }

    /* Calculate the copy length */
    int copyLen = endPos - startPos;

    /* Free the clipboard before use */
    if(clipboard != NULL)
        free(clipboard);

    /* Allocate memory for clipboard */
    clipboard = malloc(copyLen + 1);
    if(clipboard == NULL)
    {
        fprintf(stderr, "ERROR: Could not allocate memory for clipboard!\n");
        return FAILURE;
    }

    /* Copy the text into clipboard */
    memcpy(clipboard, node->text + startPos, copyLen);
    clipboard[copyLen] = '\0';

    printf("Copied \"%s\" into clipboard\n\n", clipboard);
    return SUCCESS;
}

/* Copies the range then deletes it — reuses copyText + deleteCharacters */
Status_e cutText(TextEditor *editor, int startPos, int endPos, DynamicArrayStack *undoStack)
{
   if(copyText(editor, startPos, endPos) == FAILURE)
   {
        return FAILURE;
   }

   /* deleteCharacters removes BACKWARD from pos, so pos = endPos, numChars = range length */
   int cutLen = endPos - startPos; 
   return deleteCharacters(editor, editor->cursorLine, endPos, cutLen, undoStack);
}

/* Inserts clipboard content at the current cursor position */
Status_e pasteText(TextEditor *editor, DynamicArrayStack *undoStack)
{
    if(clipboard == NULL)
    {
        fprintf(stderr, "ERROR: Clipboard is empty. Please copy text first\n");
        return FAILURE;
    }

    return insertText(editor, editor->cursorLine, editor->cursorPos, clipboard, undoStack);
}