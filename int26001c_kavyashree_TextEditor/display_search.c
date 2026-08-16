#include "display_search.h"

/* Display all lines of text in the editor and show the current cursor position. */
void displayText(TextEditor *editor) 
{
    if (editor == NULL)
        return;

    printf("\n");
    printf("====================================================\n");

    if (editor->head == NULL)
    {
        printf("Editor is empty.\n");
    }
    else
    {
        Node *temp = editor->head;
        int line = 1;

        while (temp != NULL)
        {
            printf("Line %d: %s\n",line,temp->text);
            temp = temp->next;
            line++;
        }
    }

    printf("====================================================\n");

    showCursorPosition(editor);
}

/* Display the current cursor line number and column position. */
void showCursorPosition(TextEditor *editor)
{
    if (editor == NULL)
        return;

    printf("Cursor position: Line %d, Column %d\n",editor->cursorLine,editor->cursorPos);
}

/* Search for all occurrences of a given string and display their positions. */
int findText(TextEditor *editor, const char *search)  
{
    if (editor == NULL || search == NULL || strlen(search) == 0)  
    {
        return FAILURE;
    }

    Node *temp = editor->head;
    int line = 1;
    int count = 0;

    while (temp != NULL)   
    {
        char *position = temp->text;         

        while ((position = strstr(position, search)) != NULL)  
        {
            printf("Found \"%s\" at Line %d, Column %ld\n", search,line, position - temp->text);
            count++;        
            position++;       
        }
        temp = temp->next;
        line++;
    }
    if (count == 0)
    {
        printf("Text \"%s\" not found.\n", search);
    }
    else
    {
        printf("Search completed. %d match(es) found.\n", count);
    }

    return SUCCESS;
}

/* Replace all occurrences of a given string with another string in the editor. */
int replaceText(TextEditor *editor,const char *search,const char *replace)
{
    if (editor == NULL || search == NULL || replace == NULL)
    {
        return FAILURE;
    }

    if (strlen(search) == 0)        
    {
        return FAILURE;
    }

    int oldlen = strlen(search);         
    int newlen = strlen(replace);        

    int count = 0;
    Node *temp = editor->head;
    char *position;

    while (temp != NULL)
    {
        char *start = temp->text;          

        while ((position = strstr(start, search)) != NULL)  
        {
            int prefixLength = position - temp->text;        

            int suffixLength = strlen(position + oldlen);   

            char *newText = malloc(prefixLength + newlen + suffixLength + 1);    

            if (newText == NULL)
            {
                return FAILURE;
            }

            strncpy(newText,temp->text,prefixLength);   

            strcpy(newText + prefixLength,replace);    

            strcpy(newText + prefixLength + newlen, position + oldlen);  

            free(temp->text);   

            temp->text = newText;       
            count++;

            start = temp->text + prefixLength + newlen; 
        }
        temp = temp->next;
    }

    if (count == 0)
    {
        printf("Text \"%s\" not found.\n",  search);
        return SUCCESS;
    }
    editor->modified = 1;     

    printf("Replaced %d occurrence(s).\n", count);

    return SUCCESS;
}
