#include "file_ops.h"
#include "redo_undo.h"

/* Open a file, load its contents into the editor, and reset the editor state. */
int openFile(TextEditor *editor, const char *filename) 
{
    char buffer[1024];

    if(editor == NULL || filename == NULL)
    {
        return FAILURE;
    }

    FILE *fp = fopen(filename, "r");   

    if (fp == NULL)
    {
        printf("Error: File '%s' not found.\n", filename);
        return FAILURE;
    }

    freeAllNodes(editor);        

    while (fgets(buffer,sizeof(buffer),fp) != NULL) 
    {
        buffer[strcspn(buffer, "\n")] = '\0';   

        Node *newNode = createNode(buffer);     

        if (newNode == NULL)
        {
            fclose(fp);
            return FAILURE;
        }

        if (editor->head == NULL)     
        {
            editor->head = newNode;
            editor->tail = newNode;
        }
        else
        {
            newNode->prev = editor->tail;     
            editor->tail->next = newNode;
            editor->tail = newNode;
        }
    }
    fclose(fp);
    free(editor->filename);    

    editor->filename = malloc(strlen(filename) + 1);  

    if (editor->filename == NULL)
    {
        return FAILURE;
    }
    strcpy(editor->filename, filename);

    editor->cursor = editor->head;       
    editor->cursorLine = 1;
    editor->cursorPos = 0;

    editor->modified = 0;   

    clearStack(&editor->undoStack);   
    clearStack(&editor->redoStack);

    initializeStack(&editor->undoStack);  
    initializeStack(&editor->redoStack);

    printf("File '%s' opened successfully.\n", filename);

    return SUCCESS;
}


/* Save the editor contents to the specified filename or the currently opened file. */
int saveFile(TextEditor *editor,const char *filename)  
{
    const char *saveName;
    if (editor == NULL)
        return FAILURE;

    if (filename != NULL &&strlen(filename) > 0)
    {
        saveName = filename;        
    }
    else
    {
        saveName = editor->filename;   
    }
    if (saveName == NULL)
    {
        printf("Error: No filename specified.\n");
        return FAILURE;
    }
    FILE *fp = fopen(saveName, "w");
    if (fp == NULL)
    {
        printf("Error: Unable to save file '%s'.\n", saveName);
        return FAILURE;
    }

    Node *temp = editor->head;

    while (temp != NULL)
    {
        fprintf(fp, "%s\n", temp->text);   
        temp = temp->next;
    }

    fclose(fp);

    if (filename != NULL && strlen(filename) > 0)   
    {
        free(editor->filename);

        editor->filename = malloc(strlen(filename) + 1);  

        if (editor->filename == NULL)
            return FAILURE;

        strcpy(editor->filename, filename);
    }

    editor->modified = 0;

    printf("File saved successfully as '%s'\n", saveName);

    return SUCCESS;
}

/* Close the currently opened file and release all associated resources. */
int closeFile(TextEditor *editor)
{
    if (editor == NULL)
        return FAILURE;

    if (editor->head == NULL)
    {
        printf("No file is currently open.\n");
        return FAILURE;
    }

    freeAllNodes(editor); 

    free(editor->filename);   

    editor->filename = NULL;

    free(editor->clipboard); 

    editor->clipboard = NULL;

    editor->cursorLine = 1;  
    editor->cursorPos = 0;

    editor->modified = 0;

    clearStack(&editor->undoStack);  
    clearStack(&editor->redoStack);

    initializeStack(&editor->undoStack);
    initializeStack(&editor->redoStack);


    printf("File closed successfully.\n");

    return SUCCESS;
}