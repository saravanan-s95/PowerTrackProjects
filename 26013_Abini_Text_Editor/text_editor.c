#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text_editor.h"
#include "text_ops.h"
#include "cursor_navigations.h"
#include "file_ops.h"
#include "display_search.h"
#include "redo_undo.h"

/* Shared data used by clipboard, undo and redo features. */
static char *clipboard = NULL;
static DynamicArrayStack undoStack;
static DynamicArrayStack redoStack;

/* Makes a separate copy of a string so the editor can store it safely. */
static char *copyString(const char *text)
{
    char *copy;

    copy = malloc(strlen(text) + 1);

    if (copy != NULL)
        strcpy(copy, text);

    return copy;
}

/* Finds and returns the node that belongs to the requested line number. */
static Node *getLine(TextEditor *editor, int line)
{
    Node *temp;
    int i;

    if (editor == NULL || line < 1)
        return NULL;

    temp = editor->head;

    for (i = 1; temp != NULL && i < line; i++)
        temp = temp->next;

    return temp;
}

/* Clears the clipboard and releases the memory used by the copied text. */
static void clearClipboard(void)
{
    free(clipboard);
    clipboard = NULL;
}

/* Starts an empty stack for storing undo or redo actions. */
void initStack(DynamicArrayStack *stack)
{
    stack->actions = NULL;
    stack->size = 0;
    stack->capacity = 0;
}

/* Frees all actions stored in the stack and resets it to an empty state. */
void clearStack(DynamicArrayStack *stack)
{
    int i;

    if (stack == NULL)
        return;

    for (i = 0; i < stack->size; i++)
    {
        free(stack->actions[i].operation);
        free(stack->actions[i].text);
    }

    free(stack->actions);

    stack->actions = NULL;
    stack->size = 0;
    stack->capacity = 0;
}

/* Stores an operation and its details at the top of the stack. */
int pushAction(DynamicArrayStack *stack, const char *operation,
               const char *text, int cursorLine, int cursorPos)
{
    Action *newActions;
    Action action;

    if (stack == NULL || operation == NULL || text == NULL)
        return 0;

    if (stack->size == stack->capacity)
    {
        int newCapacity;

        if (stack->capacity == 0)
            newCapacity = 4;
        else
            newCapacity = stack->capacity * 2;

        newActions = realloc(stack->actions,
                             newCapacity * sizeof(Action));

        if (newActions == NULL)
            return 0;

        stack->actions = newActions;
        stack->capacity = newCapacity;
    }

    action.operation = copyString(operation);
    action.text = copyString(text);
    action.cursorLine = cursorLine;
    action.cursorPos = cursorPos;

    if (action.operation == NULL || action.text == NULL)
    {
        free(action.operation);
        free(action.text);
        return 0;
    }

    stack->actions[stack->size] = action;
    stack->size++;

    return 1;
}

/* Takes the most recent action out of the stack. */
int popAction(DynamicArrayStack *stack, Action *action)
{
    if (stack == NULL || action == NULL || stack->size == 0)
        return 0;

    stack->size--;
    *action = stack->actions[stack->size];

    return 1;
}

/* Records an operation so it can be undone later. */
static void recordAction(const char *operation, const char *text,
                         int line, int pos)
{
    if (pushAction(&undoStack, operation, text, line, pos))
        clearStack(&redoStack);
}

/* Inserts text at a particular line and character position. */
static void insertAt(TextEditor *editor, int line, int pos,
                     const char *text)
{
    Node *node;
    Node *temp;
    char *newText;
    int length;
    int size;

    if (editor == NULL || text == NULL || line < 1 || pos < 0)
        return;

    size = strlen(text);

    if (size == 0)
        return;

    temp = getLine(editor, line);

    if (temp == NULL)
    {
        if (editor->head != NULL || line != 1 || pos != 0)
            return;

        node = malloc(sizeof(Node));

        if (node == NULL)
            return;

        node->text = copyString(text);

        if (node->text == NULL)
        {
            free(node);
            return;
        }

        node->prev = NULL;
        node->next = NULL;

        editor->head = node;
        editor->tail = node;
        editor->cursor = node;
        editor->cursorLine = 1;
        editor->cursorPos = size;

        return;
    }

    length = strlen(temp->text);

    if (pos > length)
        return;

    newText = realloc(temp->text, length + size + 1);

    if (newText == NULL)
        return;

    temp->text = newText;

    memmove(newText + pos + size,
            newText + pos,
            length - pos + 1);

    memcpy(newText + pos, text, size);
}

/* Removes characters from a specified position in a line. */
static void deleteAt(TextEditor *editor, int line, int pos, int count)
{
    Node *temp;
    int length;

    if (editor == NULL || count <= 0)
        return;

    temp = getLine(editor, line);

    if (temp == NULL)
        return;

    length = strlen(temp->text);

    if (pos < 0 || pos >= length)
        return;

    if (count > length - pos)
        count = length - pos;

    memmove(temp->text + pos,
            temp->text + pos + count,
            length - pos - count + 1);
}


/* Replaces a section of a line with new text. */
static void replaceAt(TextEditor *editor, int line, int pos,
                      int removeCount, const char *insertText)
{
    Node *node;
    char *newText;
    int oldLength;
    int insertLength;
    int newLength;

    node = getLine(editor, line);

    if (node == NULL || insertText == NULL || pos < 0)
        return;

    oldLength = strlen(node->text);

    if (pos > oldLength)
        return;

    if (removeCount < 0)
        removeCount = 0;

    if (removeCount > oldLength - pos)
        removeCount = oldLength - pos;

    insertLength = strlen(insertText);
    newLength = oldLength - removeCount + insertLength;

    newText = malloc(newLength + 1);

    if (newText == NULL)
        return;

    memcpy(newText, node->text, pos);
    memcpy(newText + pos, insertText, insertLength);
    strcpy(newText + pos + insertLength,
           node->text + pos + removeCount);

    free(node->text);
    node->text = newText;
}


/* Applies an undo or redo action and saves its opposite operation. */
static void applyAction(TextEditor *editor, Action action,
                        DynamicArrayStack *destination, int undoing)
{
    char *firstColon;
    char *secondColon;
    char *oldText;
    char *newText;
    char *oldWord;
    char *newWord;
    int oldLength;
    int newLength;
    const char *inverse;

    if (strcmp(action.operation, "replace") == 0)
    {
        firstColon = strchr(action.text, ':');

        if (firstColon == NULL)
            goto finish;

        oldLength = atoi(action.text);
        secondColon = strchr(firstColon + 1, ':');

        if (secondColon == NULL)
            goto finish;

        newLength = atoi(firstColon + 1);
        oldText = secondColon + 1;
        newText = oldText + oldLength;

        oldWord = malloc(oldLength + 1);
        newWord = malloc(newLength + 1);

        if (oldWord == NULL || newWord == NULL)
        {
            free(oldWord);
            free(newWord);
            goto finish;
        }

        memcpy(oldWord, oldText, oldLength);
        oldWord[oldLength] = '\0';
        memcpy(newWord, newText, newLength);
        newWord[newLength] = '\0';

        if (undoing)
            replaceAt(editor, action.cursorLine, action.cursorPos,
                      newLength, oldWord);
        else
            replaceAt(editor, action.cursorLine, action.cursorPos,
                      oldLength, newWord);

        free(oldWord);
        free(newWord);

        pushAction(destination, "replace", action.text,
                   action.cursorLine, action.cursorPos);
    }
    else
    {
        if (strcmp(action.operation, "insert") == 0)
        {
            deleteAt(editor, action.cursorLine,
                     action.cursorPos, strlen(action.text));

            inverse = "delete";
        }
        else
        {
            insertAt(editor, action.cursorLine,
                     action.cursorPos, action.text);

            inverse = "insert";
        }

        pushAction(destination, inverse, action.text,
                   action.cursorLine, action.cursorPos);
    }

    if (editor != NULL)
    {
        Node *node = getLine(editor, action.cursorLine);

        if (node != NULL)
        {
            editor->cursor = node;
            editor->cursorLine = action.cursorLine;
            editor->cursorPos = action.cursorPos;
            if ((undoing && strcmp(action.operation, "delete") == 0) ||
                (!undoing && strcmp(action.operation, "delete") == 0))
                editor->cursorPos += (int)strlen(action.text);
        }
        else if (editor->head == NULL)
        {
            editor->cursor = NULL;
            editor->cursorLine = 1;
            editor->cursorPos = 0;
        }
    }

finish:
    free(action.operation);
    free(action.text);
}

/* Reverses the most recent editing operation. */
void undoAction(TextEditor *editor)
{
    Action action;

    if (!popAction(&undoStack, &action))
    {
        printf("Nothing to undo.\n");
        return;
    }

    applyAction(editor, action, &redoStack, 1);
    printf("Undo completed.\n");
}

/* Applies again the most recent operation that was undone. */
void redoAction(TextEditor *editor)
{
    Action action;

    if (!popAction(&redoStack, &action))
    {
        printf("Nothing to redo.\n");
        return;
    }

    applyAction(editor, action, &undoStack, 0);
    printf("Redo completed.\n");
}

/* Inserts text at the current cursor position within a line. */
void insertText(TextEditor *editor, const char *text)
{
    char *newText;
    int length;
    int size;
    int line;
    int pos;

    if (editor == NULL || text == NULL || text[0] == '\0')
        return;

    size = strlen(text);

    if (editor->cursor == NULL)
    {
        if (editor->head != NULL)
            return;

        recordAction("insert", text, 1, 0);
        insertAt(editor, 1, 0, text);
        return;
    }

    length = strlen(editor->cursor->text);

    if (editor->cursorPos < 0 || editor->cursorPos > length)
        return;

    line = editor->cursorLine;
    pos = editor->cursorPos;

    newText = realloc(editor->cursor->text, length + size + 1);

    if (newText == NULL)
        return;

    editor->cursor->text = newText;

    memmove(newText + pos + size,
            newText + pos,
            length - pos + 1);

    memcpy(newText + pos, text, size);

    editor->cursorPos += size;

    recordAction("insert", text, line, pos);
}

/* Deletes the requested number of characters from the cursor position. */
void deleteCharacters(TextEditor *editor, int count)
{
    int length;
    int line;
    int pos;
    char *deletedText;

    if (editor == NULL || editor->cursor == NULL || count <= 0)
        return;

    length = strlen(editor->cursor->text);

    if (editor->cursorPos < 0 || editor->cursorPos >= length)
        return;

    if (count > length - editor->cursorPos)
        count = length - editor->cursorPos;

    line = editor->cursorLine;
    pos = editor->cursorPos;

    deletedText = malloc(count + 1);

    if (deletedText == NULL)
        return;

    memcpy(deletedText, editor->cursor->text + pos, count);
    deletedText[count] = '\0';

    recordAction("delete", deletedText, line, pos);

    memmove(editor->cursor->text + pos,
            editor->cursor->text + pos + count,
            length - pos - count + 1);

    free(deletedText);
}

/* Copies characters from the cursor position into the clipboard. */
void copyText(TextEditor *editor, int count)
{
    int length;
    char *newClipboard;

    if (editor == NULL || editor->cursor == NULL || count <= 0)
        return;

    length = strlen(editor->cursor->text);

    if (editor->cursorPos < 0 || editor->cursorPos >= length)
        return;

    if (count > length - editor->cursorPos)
        count = length - editor->cursorPos;

    newClipboard = malloc(count + 1);

    if (newClipboard == NULL)
        return;

    memcpy(newClipboard, editor->cursor->text + editor->cursorPos, count);
    newClipboard[count] = '\0';

    free(clipboard);
    clipboard = newClipboard;

    printf("Text copied.\n");
}

/* Copies characters to the clipboard and then deletes them. */
void cutText(TextEditor *editor, int count)
{
    if (editor == NULL || editor->cursor == NULL || count <= 0)
        return;

    copyText(editor, count);
    deleteCharacters(editor, count);
}

/* Inserts the clipboard contents at the current cursor position. */
void pasteText(TextEditor *editor)
{
    if (clipboard == NULL)
    {
        printf("Clipboard is empty.\n");
        return;
    }

    insertText(editor, clipboard);
}

/* Removes the line currently selected by the cursor. */
void deleteLine(TextEditor *editor)
{
    Node *temp;

    if (editor == NULL || editor->cursor == NULL)
        return;

    temp = editor->cursor;

    clearStack(&undoStack);
    clearStack(&redoStack);

    if (temp->prev != NULL)
        temp->prev->next = temp->next;
    else
        editor->head = temp->next;

    if (temp->next != NULL)
        temp->next->prev = temp->prev;
    else
        editor->tail = temp->prev;

    if (temp->next != NULL)
    {
        editor->cursor = temp->next;
    }
    else
    {
        editor->cursor = temp->prev;

        if (editor->cursorLine > 1)
            editor->cursorLine--;
    }

    free(temp->text);
    free(temp);

    editor->cursorPos = 0;

    if (editor->cursor == NULL)
        editor->cursorLine = 1;
}

/* Displays the document and marks the cursor with a vertical bar. */
void displayText(TextEditor *editor)
{
    Node *temp;
    int line = 1;
    int i;
    int length;

    if (editor == NULL || editor->head == NULL)
    {
        printf("Text editor is empty.\n");
        printf("Cursor position: Line 1, Column 1\n");
        return;
    }

    printf("\n----------------------------------------\n");
    temp = editor->head;

    while (temp != NULL)
    {
        length = strlen(temp->text);
        printf("Line %d: ", line);

        for (i = 0; i <= length; i++)
        {
            if (editor->cursor == temp && editor->cursorPos == i)
                printf("|");

            if (i < length)
                putchar(temp->text[i]);
        }

        putchar('\n');
        temp = temp->next;
        line++;
    }

    printf("----------------------------------------\n");
    printf("Cursor position: Line %d, Column %d\n",
           editor->cursorLine, editor->cursorPos);
}

/* Prints the current cursor line and character position. */
void showCursorPosition(TextEditor *editor)
{
    if (editor == NULL || editor->cursor == NULL)
    {
        printf("Cursor: no text selected.\n");
        return;
    }

    printf("Cursor line: %d, position: %d\n",
           editor->cursorLine, editor->cursorPos);
}

/* Searches every line and prints each position where the text is found. */
int findText(TextEditor *editor, const char *pattern)
{
    Node *temp;
    char *found;
    int line = 1;
    int matches = 0;

    if (editor == NULL || pattern == NULL || pattern[0] == '\0')
        return 0;

    temp = editor->head;

    while (temp != NULL)
    {
        char *start = temp->text;

        while ((found = strstr(start, pattern)) != NULL)
        {
            int pos = found - temp->text;

            printf("Found at line %d, position %d\n", line, pos);
            matches++;

            start = found + 1;
        }

        temp = temp->next;
        line++;
    }

    if (matches == 0)
        printf("Text not found.\n");

    return matches;
}


/* Replaces occurrences of the search text and records them for undo. */
int replaceText(TextEditor *editor, const char *pattern,
                const char *replacement)
{
    Node *temp;
    char *found;
    char *start;
    char *historyText;
    char header[50];
    int line = 1;
    int matches = 0;
    int patternLength;
    int replacementLength;
    int headerLength;
    size_t historyLength;

    if (editor == NULL || pattern == NULL ||
        replacement == NULL || pattern[0] == '\0')
        return 0;

    patternLength = strlen(pattern);
    replacementLength = strlen(replacement);

    temp = editor->head;

    while (temp != NULL)
    {
        start = temp->text;

        while ((found = strstr(start, pattern)) != NULL)
        {
            int pos = found - temp->text;

            headerLength = snprintf(header, sizeof(header),
                                    "%d:%d:",
                                    patternLength, replacementLength);

            if (headerLength < 0 ||
                headerLength >= (int)sizeof(header))
                return matches;

            historyLength = headerLength +
                            patternLength + replacementLength;

            historyText = malloc(historyLength + 1);

            if (historyText == NULL)
                return matches;

            memcpy(historyText, header, headerLength);
            memcpy(historyText + headerLength, pattern,
                   patternLength);
            memcpy(historyText + headerLength + patternLength,
                   replacement, replacementLength);

            historyText[historyLength] = '\0';

            if (!pushAction(&undoStack, "replace", historyText,
                            line, pos))
            {
                free(historyText);
                return matches;
            }

            free(historyText);
            clearStack(&redoStack);

            deleteAt(editor, line, pos, patternLength);
            insertAt(editor, line, pos, replacement);

            matches++;
            start = temp->text + pos + replacementLength;
        }

        temp = temp->next;
        line++;
    }

    if (matches == 0)
        printf("No matching text found.\n");
    else
        printf("Replaced %d occurrence(s).\n", matches);

    return matches;
}

/* Moves the cursor to the previous line when one is available. */
void moveCursorUp(TextEditor *editor)
{
    int length;

    if (editor == NULL || editor->cursor == NULL ||
        editor->cursor->prev == NULL)
        return;

    editor->cursor = editor->cursor->prev;
    editor->cursorLine--;

    length = strlen(editor->cursor->text);

    if (editor->cursorPos > length)
        editor->cursorPos = length;
}

/* Moves the cursor to the next line when one is available. */
void moveCursorDown(TextEditor *editor)
{
    int length;

    if (editor == NULL || editor->cursor == NULL ||
        editor->cursor->next == NULL)
        return;

    editor->cursor = editor->cursor->next;
    editor->cursorLine++;

    length = strlen(editor->cursor->text);

    if (editor->cursorPos > length)
        editor->cursorPos = length;
}

/* Moves the cursor one character left, or to the previous line end. */
void moveCursorLeft(TextEditor *editor)
{
    if (editor == NULL || editor->cursor == NULL)
        return;

    if (editor->cursorPos > 0)
        editor->cursorPos--;
    else if (editor->cursor->prev != NULL)
    {
        editor->cursor = editor->cursor->prev;
        editor->cursorLine--;
        editor->cursorPos = strlen(editor->cursor->text);
    }
}

/* Moves the cursor one character right, or to the next line start. */
void moveCursorRight(TextEditor *editor)
{
    int length;

    if (editor == NULL || editor->cursor == NULL)
        return;

    length = strlen(editor->cursor->text);

    if (editor->cursorPos < length)
        editor->cursorPos++;
    else if (editor->cursor->next != NULL)
    {
        editor->cursor = editor->cursor->next;
        editor->cursorLine++;
        editor->cursorPos = 0;
    }
}

/* Places the cursor at the beginning of the document. */
void jumpToStartOfFile(TextEditor *editor)
{
    if (editor == NULL || editor->head == NULL)
        return;

    editor->cursor = editor->head;
    editor->cursorLine = 1;
    editor->cursorPos = 0;
}

/* Places the cursor at the end of the document. */
void jumpToEndOfFile(TextEditor *editor)
{
    Node *temp;

    if (editor == NULL || editor->tail == NULL)
        return;

    editor->cursor = editor->tail;
    editor->cursorPos = strlen(editor->tail->text);

    temp = editor->head;
    editor->cursorLine = 1;

    while (temp != editor->tail)
    {
        temp = temp->next;
        editor->cursorLine++;
    }
}

/* Places the cursor at the beginning of the current line. */
void jumpToStartOfLine(TextEditor *editor)
{
    if (editor != NULL && editor->cursor != NULL)
        editor->cursorPos = 0;
}

/* Places the cursor at the end of the current line. */
void jumpToEndOfLine(TextEditor *editor)
{
    if (editor != NULL && editor->cursor != NULL)
        editor->cursorPos = strlen(editor->cursor->text);
}

/* Reads a file and loads its lines into the editor. */
int openFile(TextEditor *editor, const char *filename)
{
    FILE *fp;
    char line[1000];
    Node *node = NULL;
    Node *newHead = NULL;
    Node *newTail = NULL;

    if (editor == NULL || filename == NULL || filename[0] == '\0')
        return 0;

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Unable to open file.\n");
        return 0;
    }

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        size_t length = strlen(line);

        if (length > 0 && line[length - 1] == '\n')
            line[--length] = '\0';

        node = malloc(sizeof(Node));

        if (node == NULL)
            break;

        node->text = copyString(line);

        if (node->text == NULL)
        {
            free(node);
            break;
        }

        node->prev = newTail;
        node->next = NULL;

        if (newTail != NULL)
            newTail->next = node;
        else
            newHead = node;

        newTail = node;
    }

    if (ferror(fp) || (!feof(fp) && node == NULL))
    {
        fclose(fp);

        while (newHead != NULL)
        {
            node = newHead;
            newHead = newHead->next;
            free(node->text);
            free(node);
        }

        printf("Error while reading file.\n");
        return 0;
    }

    fclose(fp);
    closeFile(editor);

    editor->head = newHead;
    editor->tail = newTail;
    editor->cursor = newHead;
    editor->cursorLine = 1;
    editor->cursorPos = 0;

    clearStack(&undoStack);
    clearStack(&redoStack);

    printf("File opened successfully.\n");
    return 1;
}

/* Writes the editor contents to a file. */
int saveFile(TextEditor *editor, const char *filename)
{
    FILE *fp;
    Node *temp;

    if (editor == NULL || filename == NULL || filename[0] == '\0')
        return 0;

    fp = fopen(filename, "w");

    if (fp == NULL)
    {
        printf("Unable to save file.\n");
        return 0;
    }

    temp = editor->head;

    while (temp != NULL)
    {
        if (fprintf(fp, "%s\n", temp->text) < 0)
        {
            fclose(fp);
            printf("Error while writing file.\n");
            return 0;
        }

        temp = temp->next;
    }

    if (fclose(fp) != 0)
    {
        printf("Error while closing file.\n");
        return 0;
    }

    printf("File saved successfully.\n");
    return 1;
}

/* Frees the document lines and clears the editor contents. */
void closeFile(TextEditor *editor)
{
    Node *temp;
    Node *next;

    if (editor == NULL)
        return;

    temp = editor->head;

    while (temp != NULL)
    {
        next = temp->next;
        free(temp->text);
        free(temp);
        temp = next;
    }

    editor->head = NULL;
    editor->tail = NULL;
    editor->cursor = NULL;
    editor->cursorLine = 1;
    editor->cursorPos = 0;
}

/* Reads a number from the user and checks that the input is numeric. */
static int readNumber(const char *message)
{
    char input[100];
    int number;
    char extra;

    printf("%s", message);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return -1;

    if (sscanf(input, " %d %c", &number, &extra) != 1)
        return -1;

    return number;
}

/* Shows the numbered menu used by the menu-based interface. */
void printMenu(void)
{
    printf("\n----- TEXT EDITOR -----\n");
    printf("1. Insert Text\n");
    printf("2. Delete Characters\n");
    printf("3. Move Cursor Up\n");
    printf("4. Move Cursor Down\n");
    printf("5. Move Cursor Left\n");
    printf("6. Move Cursor Right\n");
    printf("7. Start of File\n");
    printf("8. End of File\n");
    printf("9. Start of Line\n");
    printf("10. End of Line\n");
    printf("11. Copy Text\n");
    printf("12. Cut Text\n");
    printf("13. Paste Text\n");
    printf("14. Delete Line\n");
    printf("15. Display Text\n");
    printf("16. Open File\n");
    printf("17. Save File\n");
    printf("18. Close File\n");
    printf("19. Show Cursor Position\n");
    printf("20. Find Text\n");
    printf("21. Replace Text\n");
    printf("22. Undo\n");
    printf("23. Redo\n");
    printf("24. Exit\n");
}

/* Runs the operation selected from the numbered menu. */
void handleCommand(TextEditor *editor, int choice)
{
    char text[1000];
    char pattern[500];
    char replacement[500];
    int count;

    switch (choice)
    {
        case 1:
            printf("Enter text: ");

            if (fgets(text, sizeof(text), stdin) != NULL)
            {
                text[strcspn(text, "\n")] = '\0';
                insertText(editor, text);
            }
            break;

        case 2:
            count = readNumber("Number of characters to delete: ");
            deleteCharacters(editor, count);
            break;

        case 3:
            moveCursorUp(editor);
            break;

        case 4:
            moveCursorDown(editor);
            break;

        case 5:
            moveCursorLeft(editor);
            break;

        case 6:
            moveCursorRight(editor);
            break;

        case 7:
            jumpToStartOfFile(editor);
            break;

        case 8:
            jumpToEndOfFile(editor);
            break;

        case 9:
            jumpToStartOfLine(editor);
            break;

        case 10:
            jumpToEndOfLine(editor);
            break;

        case 11:
            count = readNumber("Number of characters to copy: ");
            copyText(editor, count);
            break;

        case 12:
            count = readNumber("Number of characters to cut: ");
            cutText(editor, count);
            break;

        case 13:
            pasteText(editor);
            break;

        case 14:
            deleteLine(editor);
            break;

        case 15:
            displayText(editor);
            break;

        case 16:
            printf("Filename to open: ");

            if (fgets(text, sizeof(text), stdin) != NULL)
            {
                text[strcspn(text, "\n")] = '\0';
                openFile(editor, text);
            }
            break;

        case 17:
            printf("Filename to save: ");

            if (fgets(text, sizeof(text), stdin) != NULL)
            {
                text[strcspn(text, "\n")] = '\0';
                saveFile(editor, text);
            }
            break;

        case 18:
            closeFile(editor);
            clearClipboard();
            clearStack(&undoStack);
            clearStack(&redoStack);
            printf("Editor content cleared.\n");
            break;

        case 19:
            showCursorPosition(editor);
            break;

        case 20:
            printf("Enter text to find: ");

            if (fgets(pattern, sizeof(pattern), stdin) != NULL)
            {
                pattern[strcspn(pattern, "\n")] = '\0';
                findText(editor, pattern);
            }
            break;

        case 21:
            printf("Enter text to find: ");

            if (fgets(pattern, sizeof(pattern), stdin) == NULL)
                break;

            pattern[strcspn(pattern, "\n")] = '\0';

            printf("Enter replacement text: ");

            if (fgets(replacement, sizeof(replacement), stdin) != NULL)
            {
                replacement[strcspn(replacement, "\n")] = '\0';
                replaceText(editor, pattern, replacement);
            }
            break;

        case 22:
            undoAction(editor);
            break;

        case 23:
            redoAction(editor);
            break;

        case 24:
            printf("Exiting Text Editor.\n");
            break;

        default:
            printf("Invalid choice.\n");
            break;
    }
}


/* Shows the commands available in the text editor. */
/* Command-line interface used by main(). */
static void printCommands(void)
{
    printf("\n========================================\n");
    printf("             TEXT EDITOR V1.0\n");
    printf("========================================\n");
    printf("Text Editor Commands:\n");
    printf("insert <text> - Insert text at cursor\n");
    printf("delete <num>  - Delete characters\n");
    printf("undo          - Undo last operation\n");
    printf("redo          - Redo last undone operation\n");
    printf("print         - Display text and cursor position\n");
    printf("exit          - Exit the text editor\n");
    printf("----------------------------------------\n");
}

/* Skips spaces and tabs at the beginning of a command. */
static char *skipSpaces(char *text)
{
    while (*text == ' ' || *text == '\t')
        text++;

    return text;
}

/* The command interface treats each insert command as a new line,
   matching the LMS example. */
static void insertCommandText(TextEditor *editor, const char *text)
{
    Node *node;
    Node *current;
    int newLine;

    if (editor == NULL || text == NULL || text[0] == '\0')
    {
        printf("Text cannot be empty.\n");
        return;
    }

    if (editor->head == NULL || editor->cursor == NULL)
    {
        insertText(editor, text);
        return;
    }

    current = editor->cursor;
    node = malloc(sizeof(Node));

    if (node == NULL)
    {
        printf("Unable to allocate memory for the new line.\n");
        return;
    }

    node->text = copyString(text);
    if (node->text == NULL)
    {
        free(node);
        printf("Unable to allocate memory for the text.\n");
        return;
    }

    node->prev = current;
    node->next = current->next;

    if (current->next != NULL)
        current->next->prev = node;
    else
        editor->tail = node;

    current->next = node;
    newLine = editor->cursorLine + 1;

    /* Record insertion at the start of the new line for undo/redo. */
    recordAction("insert", text, newLine, 0);
    editor->cursor = node;
    editor->cursorLine = newLine;
    editor->cursorPos = (int)strlen(text);
}

/* Checks the command entered by the user and runs the matching operation. */
void processCommand(TextEditor *editor, char *input)
{
    char *argument;
    char *second;
    char *end;
    int count;

    input[strcspn(input, "\r\n")] = '\0';
    input = skipSpaces(input);

    if (*input == '\0')
        return;

    if (strcmp(input, "insert") == 0 || strncmp(input, "insert ", 7) == 0)
    {
        char text[2000];

        /* Accept both "insert text" and "insert" followed by a text line. */
        if (strcmp(input, "insert") == 0)
            argument = input + strlen(input);
        else
            argument = skipSpaces(input + 7);

        if (*argument == '\0')
        {
            /* Read the text separately so it is never treated as a command. */
            printf("\nEnter text to insert: ");
            fflush(stdout);

            if (fgets(text, sizeof(text), stdin) == NULL)
            {
                printf("\nInput ended. Text was not inserted.\n");
                return;
            }

            text[strcspn(text, "\r\n")] = '\0';
            if (text[0] == '\0')
            {
                printf("No text inserted because the line was empty.\n");
                return;
            }
            insertCommandText(editor, text);
        }
        else
        {
            insertCommandText(editor, argument);
        }

        printf("Text inserted successfully.\n");
    }
    else if (strncmp(input, "delete ", 7) == 0)
    {
        argument = skipSpaces(input + 7);
        count = (int)strtol(argument, &end, 10);

        if (argument == end || *skipSpaces(end) != '\0' || count <= 0)
            printf("Usage: delete <positive number>\n");
        else if (editor->cursor == NULL || editor->cursorPos <= 0)
            printf("Nothing to delete before the cursor.\n");
        else
        {
            if (count > editor->cursorPos)
                count = editor->cursorPos;

            editor->cursorPos -= count;
            deleteCharacters(editor, count);
            printf("Deleted %d character(s) successfully.\n", count);
        }
    }
    else if (strcmp(input, "undo") == 0)
        undoAction(editor);
    else if (strcmp(input, "redo") == 0)
        redoAction(editor);
    else if (strcmp(input, "print") == 0)
        displayText(editor);
    else if (strcmp(input, "up") == 0)
        moveCursorUp(editor);
    else if (strcmp(input, "down") == 0)
        moveCursorDown(editor);
    else if (strcmp(input, "left") == 0)
        moveCursorLeft(editor);
    else if (strcmp(input, "right") == 0)
        moveCursorRight(editor);
    else if (strcmp(input, "home") == 0)
        jumpToStartOfLine(editor);
    else if (strcmp(input, "end") == 0)
        jumpToEndOfLine(editor);
    else if (strcmp(input, "top") == 0)
        jumpToStartOfFile(editor);
    else if (strcmp(input, "bottom") == 0)
        jumpToEndOfFile(editor);
    else if (strncmp(input, "copy ", 5) == 0 || strncmp(input, "cut ", 4) == 0)
    {
        int isCut = (input[1] == 'u');
        argument = skipSpaces(input + (isCut ? 4 : 5));
        count = (int)strtol(argument, &end, 10);

        if (argument == end || *skipSpaces(end) != '\0' || count <= 0)
            printf("Usage: %s <positive number>\n", isCut ? "cut" : "copy");
        else if (isCut)
            cutText(editor, count);
        else
            copyText(editor, count);
    }
    else if (strcmp(input, "paste") == 0)
        pasteText(editor);
    else if (strcmp(input, "delete-line") == 0)
    {
        deleteLine(editor);
        printf("Line deleted.\n");
    }
    else if (strncmp(input, "find ", 5) == 0)
        findText(editor, skipSpaces(input + 5));
    else if (strncmp(input, "replace ", 8) == 0)
    {
        argument = skipSpaces(input + 8);
        second = argument;

        while (*second != '\0' && *second != ' ' && *second != '\t')
            second++;

        if (*second == '\0')
            printf("Usage: replace <old_text> <new_text>\n");
        else
        {
            *second++ = '\0';
            second = skipSpaces(second);

            if (*second == '\0')
                printf("Usage: replace <old_text> <new_text>\n");
            else
                replaceText(editor, argument, second);
        }
    }
    else if (strncmp(input, "open ", 5) == 0)
    {
        argument = skipSpaces(input + 5);
        if (*argument != '\0')
            openFile(editor, argument);
        else
            printf("Usage: open <filename>\n");
    }
    else if (strncmp(input, "save ", 5) == 0)
    {
        argument = skipSpaces(input + 5);
        if (*argument != '\0')
            saveFile(editor, argument);
        else
            printf("Usage: save <filename>\n");
    }
    else if (strcmp(input, "close") == 0)
    {
        closeFile(editor);
        clearClipboard();
        clearStack(&undoStack);
        clearStack(&redoStack);
        printf("Editor content cleared.\n");
    }
    else if (strcmp(input, "help") == 0)
        printCommands();
    else if (strcmp(input, "exit") == 0)
        return;
    else
        printf("Unknown command. Type help to see commands.\n");
}

/* Starts the editor, accepts commands, and releases allocated memory before exit. */
int main(void)
{
    TextEditor editor = {0};
    char input[2000];

    initStack(&undoStack);
    initStack(&redoStack);
    printCommands();

    /* Keep reading commands until the user enters exit or input ends. */
    while (1)
    {
        printf("\nEnter command: ");

        if (fgets(input, sizeof(input), stdin) == NULL)
            break;

        input[strcspn(input, "\r\n")] = '\0';

        if (strcmp(skipSpaces(input), "exit") == 0)
            break;

        processCommand(&editor, input);
    }

    printf("Goodbye! Thank you for using Text Editor V1.0.\n");
    closeFile(&editor);
    clearClipboard();
    clearStack(&undoStack);
    clearStack(&redoStack);

    return 0;
}
