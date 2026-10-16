#include "text_editor.h"
#include <string.h>

/* Allocates a new Node, Allocated and copies initialText into it, links set to NULL */
Node* createNode(char *initialText)
{
    /* New node */
    Node *node = malloc(sizeof(Node));
    if(node == NULL) return NULL;

    /* Allocating memory for text + 1 for '\0' character */
    node->length = strlen(initialText);
    node->text = malloc(node->length + 1);
    if(node->text == NULL)
    {
        free(node);
        return NULL;
    }

    strcpy(node->text, initialText);
    node->prev = NULL;
    node->next = NULL;
    return node;
}

/* Opens filename, builds DLL from file content, sets head/tail/cursor */
Status_e openFile(TextEditor *editor, char *filename)
{
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "ERROR: File '%s' not found.\n", filename);
        return FAILURE;
    }

    editor->head = editor->tail = editor->cursor = NULL;
    char buffer[1024];
    Node *prevNode = NULL;

    /* Read file line by line until EOF */
    while(fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        /* Strip trailing newline before storing */
        int len = strlen(buffer);
        while(len > 0 && (buffer[len-1] == '\n' || buffer[len-1] == '\r'))
        {
            buffer[--len] = '\0';
        }

        Node *node = createNode(buffer);
        if(node == NULL)
        {
            fprintf(stderr, "ERROR: Failed to allocate node while opening file.\n");
            fclose(fp);
            return FAILURE;
        }

        /* Link new node to previous node, or set as head if first */
        if(prevNode == NULL)
            editor->head = node;
        else
        {
            prevNode->next = node;
            node->prev = prevNode;
        }

        prevNode = node;
    }

    /* Handle empty file — start with one blank line, same as bootstrap */
    if(editor->head == NULL)
        editor->head = createNode("");

    editor->tail = prevNode != NULL ? prevNode : editor->head;
    editor->cursor = editor->head;
    editor->cursorLine = 0;
    editor->cursorPos = 0;

    fclose(fp);
    printf("File '%s' opened successfully.\n", filename);
    return SUCCESS;
}

/* Writes DLL content back to filename, one line per node */
Status_e saveFile(TextEditor *editor, char *filename)
{
    FILE *fp = fopen(filename, "w");
    if(fp == NULL)
    {
        fprintf(stderr, "ERROR: Could not open '%s' for writing.\n", filename);
        return FAILURE;
    }

    Node *node = editor->head;
    while(node != NULL)
    {
        fprintf(fp, "%s\n", node->text);
        node = node->next;
    }

    fclose(fp);
    printf("File saved successfully as '%s'.\n", filename);
    return SUCCESS;
}

/* Frees every node in the DLL, resets editor to empty state */
Status_e closeFile(TextEditor *editor)
{
    Node *node = editor->head;
    while(node != NULL)
    {
        Node *next = node->next;
        free(node->text);
        free(node);
        node = next;
    }

    editor->head = editor->tail = editor->cursor = NULL;
    editor->cursorLine = 0;
    editor->cursorPos = 0;

    printf("File closed successfully.\n");
    return SUCCESS;
}