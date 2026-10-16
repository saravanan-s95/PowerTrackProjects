/*
    Future improvement for supporting multiline delete

1. Instead of tagging each `Action` as fully independent, add a `groupId` field (or similar) to the `Action` struct
2. Every `Action` created from the *same* user-level event (e.g., one backspace that merges two lines) shares the same `groupId`
3. A normal single-char insert/delete just gets its own unique `groupId` — nothing changes for the simple case
4. On a merge event specifically, push multiple `Action`s in sequence (e.g., one recording "line 2 deleted," one recording "line 1's text before merge") — all tagged with the same `groupId`
5. `undo()` changes slightly: instead of popping and reversing just one `Action`, it pops the top `Action`, checks its `groupId`, and keeps popping + reversing more `Action`s **as long as the next one has the same `groupId`**
6. Once the `groupId` changes (or the stack empties), `undo` stops — the whole group has been reversed as one atomic unit
7. `redo` works the same way in reverse — replay all `Action`s sharing a `groupId` together, not just one


*/

#include "text_editor.h"
#include "redo_undo.h"
#include <string.h>


/* Given the newText message and cursor, the function inserts newText at cursor location */
Status_e insertText(TextEditor *editor, int line, int pos, char *newText, DynamicArrayStack *undoStack)
{
    /* EDGE CASE : Empty string */
    if(*newText == '\0')
        return FAILURE;

    /* Finds the cursor node we wish to insert data in */
    Node *node = getNodeAtLine(editor, line);
    if(node == NULL) return FAILURE;

    /* EDGE CASE: Out of bound pos */
    if(pos > node->length)
    {
        fprintf(stderr, "ERROR: Out of Bounds cursor position for insert text operation!\n");
        return FAILURE;
    }

    /* Expand line to make space for newText */
    int text_len = strlen(newText);
    int new_len = node->length + text_len + 1;   /* +1 for null terminator */

    char *temp_text = node->text;
    temp_text = realloc(temp_text, new_len);
    if(temp_text == NULL)
    {
        fprintf(stderr, "ERROR: Failed to make additional space for insertText!\n");
        return FAILURE;
    }
    node->text = temp_text;

    /* Shift everything from pos till end (including null terminator) by text_len steps forward */
    memmove(node->text + pos + text_len,
            node->text + pos,
            node->length - pos + 1);   /* +1 carries the '\0' along with the suffix */

    /* Copy the new text into the gap.
       Using memcpy (not strcpy) — strcpy would additionally write its own '\0' 
       memcpy writes exactly text_len bytes and nothing more. */
    memcpy(node->text + pos, newText, text_len);

    /* Update length (new_len includes the +1 for '\0', so subtract it back out) */
    node->length = new_len - 1;

    /* Update the Cursor information */
    editor->cursor = node;
    editor->cursorLine = line;
    editor->cursorPos = pos + text_len;

    /* Record the action in undoStack */
    Action action;
    action.cursorLine = line;
    action.cursorPos = pos;
    action.operation = 'I';

    /* action.text must own its own memory — it outlives this function call and
       newText's storage may be reused/freed by the caller afterward. */
    action.text = malloc(text_len + 1);
    if(action.text == NULL)
    {
        fprintf(stderr, "ERROR: Failed to allocate memory for action.text!\n");
        return FAILURE;
    }
    strcpy(action.text, newText);

    push_st(undoStack, action);

    return SUCCESS;

}


/* Given numChars, deletes that many characters immediately BEFORE pos
   (backspace-style, matching the spec's sample: cursor at end, delete
   removes the preceding characters) */
Status_e deleteCharacters(TextEditor *editor, int line, int pos, int numChars, DynamicArrayStack *undoStack)
{
    /* Finds the cursor node we wish to delete data from */
    Node *node = getNodeAtLine(editor, line);
    if(node == NULL) return FAILURE;

    /* Validate if enough text is there to delete before pos */
    if(numChars > pos)
    {
        fprintf(stderr, "ERROR: Cant delete more than %d characters\n", pos);
        return FAILURE;
    }

    /* Store the text (for undo) before deleting — points to pos - numChars,
       the start of the chunk about to be removed */
    char *savedText = malloc(numChars + 1);   /* +1 for null terminator */
    if(savedText == NULL)
    {
        fprintf(stderr, "ERROR: Could not allocate memory for storing text in delete function!\n");
        return FAILURE;
    }
    memcpy(savedText, node->text + (pos - numChars), numChars);
    savedText[numChars] = '\0';   /* terminate manually — memcpy doesn't add this */

    /* Shift everything from pos till end (including null terminator) back by
       numChars steps, closing the gap left by the deleted chunk */
    memmove(node->text + (pos - numChars),
            node->text + pos,
            node->length - pos + 1);   /* +1 carries the '\0' along with the suffix */

    /* Update length and cursor pos */
    node->length -= numChars;
    editor->cursor = node;
    editor->cursorLine = line;
    editor->cursorPos = pos - numChars;

    /* Record the action in undoStack — cursorPos here must be where the
       deleted text STARTED (pos - numChars), since that's exactly where
       undo's insertText needs to put it back */
    Action action;
    action.cursorLine = line;
    action.cursorPos = pos - numChars;
    action.operation = 'D';
    action.text = savedText;

    push_st(undoStack, action);

    return SUCCESS;

}

Node* getNodeAtLine(TextEditor *editor, int line)
{
    Node *temp_cursor = editor->cursor;
    int temp_line = editor->cursorLine;

    while(1)
    {
        /* Traverse down if line is lesser than current cursor line */
        if(line < temp_line)
        {
            if(temp_cursor->prev == NULL)
            {
                fprintf(stderr, "ERROR: Invalid Line, please give positive line value!\n");
                return NULL;
            }

            temp_cursor = temp_cursor->prev;
            temp_line--;
        }
        else if(line > temp_line)
        {
            if(temp_cursor->next == NULL)
            {
                fprintf(stderr, "ERROR: Invalid Line, please enter line number within boundary!\n");
                return NULL;
            }

            temp_cursor = temp_cursor->next;
            temp_line++;
        }
        else
        {
            return temp_cursor;
        }
    }
}


/* Prints one line, highlighting the character at cursorPos if this is the cursor's line */
void printLineWithCursor(Node *node, int lineNum, int isCursorLine, int cursorPos)
{
    /* Label color changes if this is the cursor's current line */
    printf("%s Line %d: %s\"", isCursorLine ? COLOR_CURSOR_LINE_LABEL : COLOR_LABEL, lineNum, COLOR_TEXT);

    for(int i = 0; i <= node->length; i++)
    {
        if(isCursorLine && i == cursorPos)
        {
            printf("\033[5m\033[7m");   /* blink + reverse-video for the cursor character */
        }

        if(i < node->length)
            putchar(node->text[i]);
        else
            putchar(' ');   /* cursor at end-of-line */

        if(isCursorLine && i == cursorPos)
        {
            printf(COLOR_RESET COLOR_TEXT);   /* reset highlight, stay in text color */
        }
    }

    printf("\"%s (len=%d)%s\n", COLOR_LENGTH, node->length, COLOR_RESET);
}


/* Walks the list and prints every line plus current cursor state */
void printEditor(TextEditor *editor)
{
    system("clear");

    Node *temp = editor->head;
    int lineNum = 0;

    printf("%s---- Editor state ----%s\n", COLOR_HEADER, COLOR_RESET);
    while (temp != NULL)
    {
        int isCursorLine = (lineNum == editor->cursorLine);
        printLineWithCursor(temp, lineNum, isCursorLine, editor->cursorPos);
        temp = temp->next;
        lineNum++;
    }
    printf("%sCursor -> line %d, pos %d%s\n", COLOR_STATUS, editor->cursorLine, editor->cursorPos, COLOR_RESET);
    printf("%s-----------------------%s\n\n", COLOR_HEADER, COLOR_RESET);
}
