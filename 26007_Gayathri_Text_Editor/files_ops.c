#include "files_ops.h"
#include "text_editor.h"
#include "redo_undo.h"
#include <stdio.h>

//Function to save the editor content to a file
void saveToFile(TextEditor *ed, const char *filename) 
{
    //check if filename and editor is invalid
    if (!ed || !filename) 
        return;

    //open the file in write mode    
    FILE *fp = fopen(filename, "w");

    //check if file could not be opened
    if (!fp) 
    {
        //print error message
        printf("Error: Cannot write %s\n", filename);
        return;
    }

    //start from the head
    Node *temp = ed->head; 


    while (temp) 
    {
        //check curr line as text or not
        if (temp->text)
            //write the line text to the file new line
            fprintf(fp, "%s\n", temp->text);
        else
            //if line is empty
            fprintf(fp, "\n");


        //move to next line    
        temp = temp->next;
    }

    //close the file
    fclose(fp);

    //print the message of totallines in filename
    printf("Saved %d lines to %s\n", ed->totallines, filename);
}


//Function to load the editor content to a file
int loadFromFile(TextEditor *ed, const char *filename) 
{
    // //check if filename and editor is invalid
    if (!ed || !filename) 
    
        return 0;

    //open the file in read mode        
    FILE *fp = fopen(filename, "r");
    
    //check if file could not be opened
    if (!fp) 
    {
        
        return 0;
    }
    
    //start from the head
    Node *curr = ed->head;

    while(curr) 
    {
        //save the next node before deleting the curr node
        Node *next = curr->next;

        //check if curr node has text allocated
        if(curr->text) 

            //free the memory used for text
            free(curr->text);

        //free the memory used for curr node    
        free(curr);

        //move to next node to continue
        curr = next;
    }

    //reset all editor pointer to NULL
    ed->head = ed->tail = ed->cursor = NULL;

    //reset total line count to 0
    ed->totallines = 0;

    //reset cursor line pos to 0
    ed->cursorLine = 0;

    //reset cursor col pos to 0
    ed->cursorPos = 0;

    //buffer to store the line read from the file
    char line[1024];

    //read the file line by line
    while (fgets(line, sizeof(line), fp)) 
    {

        //remove the newline char
        line[strcspn(line, "\n")] = 0; 


        //create new node for a new line
        Node *newNode = createNode(line);

        //check if this is a first line
        if (!ed->head) 
        {

            //first node becomes head
            ed->head = newNode;
            ed->tail =  newNode;
            ed->cursor = newNode;
        } 
        else
        {
            //link new node after the curr tail
            newNode->prev = ed->tail;
            ed->tail->next = newNode;
           
        }
        //increase the totalline by count 1
        ed->totallines++;
    }

    //close the file
    fclose(fp);
    
    return 1;
}