#include "header.h"
 

int main(void)
{
    TxtEdtr editor;
    DynArrStk undoStack;
    DynArrStk redoStack;
    char command[2048];

    initEditor(&editor);
    initStack(&undoStack);
    initStack(&redoStack);

    system("clear");
    printf("============================================\n");
    printf("             TEXT EDITOR\n");
    printf("============================================\n");
    printf("Type 'help' for available commands.\n");

    while (1)
    {
        printf("\nEnter command: ");
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }
        
        if (!strcmp(command, "exit\n"))
        {
            break;
        }

        handleCommand(&editor, &undoStack, &redoStack, command);
    }

    freeEditor(&editor);
    freeStack(&undoStack);
    freeStack(&redoStack);

    printf("Goodbye! Thank you for using Text Editor\n");
    return 0;
}
