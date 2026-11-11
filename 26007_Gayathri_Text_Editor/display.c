#include "display.h"


//function to display the entire text editor screen
void display(TextEditor *ed)
{
    //print the header with total lines and current cursor position
    printf("\n== TEXT EDITOR == Lines:%d Cursor:(%d %d) == \n ", ed->totallines, ed->cursorLine, ed->cursorPos);

    //start from the head
    Node *c =  ed->head;

    //line number counter starts from 0
    int i = 0;
    
    while(c)
    {
        //check if this is the curr line , where the cursor is
        if(c == ed->cursor)
        {
            //print the curr line with its line number
            printf("%3d> %s\n", i, c->text);

            //print spaces to align
            printf("     ");

            //print spaces up to cursor pos
            for(int k = 0; k < ed->cursorPos;k++)
                printf("  ");

            //print the cursor symbol (^) below the cursor pos    
            printf("^\n");    
        }
        else
        {
            //print normal line without cursor indicator
            printf("%3d %s\n", i, c->text);

        }

        //move to next line
        c = c->next;

        //increase the line number
        i++;

    }

    
}