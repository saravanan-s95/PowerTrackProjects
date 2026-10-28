#include "header.h"


/*
Functions in this file:
    void displayText(TxtEdtr *editor);
    void showCursorPosition(TxtEdtr *editor);
    int findText(TxtEdtr *editor,const char *search);
    int replaceText(TxtEdtr *editor,const char *search,const char *replace);
*/


void displayText(TxtEdtr *editr)
{
    Node *crnt;
    int line = 1;
    printf("\n============================================\n");
    printf("              TEXT EDITOR\n");
    printf("============================================\n");

    crnt = editr->head;
    if (crnt == NULL)
        printf("Editor is empty.\n");
    

    while (crnt != NULL)
    {
        if(crnt->txt != NULL)
            printf("Line %d: %s\n", line, crnt->txt);   
        else
            printf("Line %d: \n", line);
        fflush(stdout);
        crnt = crnt->nxt;
        line++;
    }

    showCursorPosition(editr);
    printf("============================================\n");
}

void showCursorPosition(TxtEdtr *editr)
{
    if (editr == NULL || editr->cursor == NULL)
    {
        printf("Cursor: No position\n");
        return;
    }

    printf("--------------------------------------------\n");
    printf("Cursor position: Line %d, Column %d\n",editr->cline,editr->cpos);
}

int findText(TxtEdtr *editr, const char *search)
{
    if (editr == NULL || search == NULL || search[0] == '\0')
    {
        printf("Error: Search text cannot be empty.\n");
        return 0;
    }

    int line = 1;
    int count = 0;
    char *position;
    Node *crnt = editr->head;   

    while (crnt != NULL)
    {
        position = crnt->txt;

        while ( (position = strstr(position, search)) != NULL )
        {
            printf("Found \"%s\" at Line %d, Column %ld\n",search, line, (long)(position - crnt->txt));
            count++;
            position++;
        }
        crnt = crnt->nxt;
        line++;
    }

    printf("Search completed. %d matches found.\n", count);
    return count;
}

int replaceText(TxtEdtr *editr, const char *search, const char *replace)
{
    if (editr == NULL || search == NULL || replace == NULL || search[0] == '\0')
    {
        printf("Error: Invalid search or replacement text.\n");
        return 0;
    }


    Node *crnt = editr->head;
    size_t sLen = strlen(search);
    size_t replaceLen = strlen(replace);
    int count = 0;
    
    while (crnt != NULL)
    {
        char *s = crnt->txt;

        while ((s = strstr(s, search)) != NULL)
        {
            size_t oldLen = strlen(crnt->txt);           // old length of line
            size_t prefixLen = (size_t)(s - crnt->txt);  // length before word( need to preserve it)
            size_t newLen = oldLen - sLen + replaceLen;  // new length of line
            
            char *newText = malloc(newLen + 1);
            if (newText == NULL)
            {
                printf("Error: Memory allocation failed.\n");
                return count;
            }

            memcpy(newText, crnt->txt, prefixLen);                // copying "word before characters"
            memcpy(newText + prefixLen, replace, replaceLen);     // replacing new word on place of old word
            strcpy(newText + prefixLen + replaceLen, s + sLen);   // copying remaining "word after characters"
            free(crnt->txt);
            crnt->txt = newText;
            count++;

            s = crnt->txt + prefixLen + replaceLen;    //Continue after the newly inserted replacement.
        }

        crnt = crnt->nxt;
    }

    printf("Replaced %d occurrences.\n", count);
    return count;
}
