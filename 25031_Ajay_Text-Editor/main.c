#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "text_editor.h"
#include "colour.h"

int main()
{
    printf(BLUE "-----------------------------------\n");
    printf("  T E X T   E D I T O R\n");
    printf("-----------------------------------\n");
    while (1)
    {
        char commands[40], text[50];
        int choice = 0, line_no;

        printf("%sINSERT%s -> PRESS %s1%s (OR) Type %sInsert%s\n", RED, VIOLET, VIOLET, VIOLET, VIOLET, VIOLET);
        printf("%sDELETE%s -> PRESS %s2%s (OR) Type %sDelete%s\n", RED, VIOLET, VIOLET, VIOLET, VIOLET, VIOLET);
        printf("%sUNDO%s   -> PRESS %s3%s (OR) Type %sUndo%s\n", RED, VIOLET, VIOLET, VIOLET, VIOLET, VIOLET);
        printf("%sREDO%s   -> PRESS %s4%s (OR) Type %sRedo%s\n", RED, VIOLET, VIOLET, VIOLET, VIOLET, VIOLET);
        printf("%sPRINT%s  -> PRESS %s5%s (OR) Type %sPrint%s\n", RED, VIOLET, VIOLET, VIOLET, VIOLET, VIOLET);
        printf("%sSAVE%s   -> PRESS %s6%s (OR) Type %sSave%s\n", RED, VIOLET, VIOLET, VIOLET, VIOLET, VIOLET);
        printf("%sOPEN%s   -> PRESS %s7%s (OR) Type %sOpen%s\n", RED, VIOLET, VIOLET, VIOLET, VIOLET, VIOLET);
        printf("%sEXIT%s   -> PRESS %s8%s (OR) Type %sExit%s\n", RED, VIOLET, VIOLET, VIOLET, VIOLET, VIOLET);

        printf("-----------------------------------\n");
        printf(GREEN "Choose (Type Command or Enter Number) : ");
        scanf("%s", commands);
        // convert ascii to integer incase user enters integer
        choice = atoi(commands);
        if (choice == 1 || strcmp(commands, "Insert") == 0)
        {
            insert_text();
            printf("\n✅ Insertion Successfully Done ✅\n\n");
        }
        else if (choice == 2 || strcmp(commands, "Delete") == 0)
        {
            delete_text(line_no);
        }
        else if (choice == 3 || strcmp(commands, "Undo") == 0)
        {
            undo();
        }
        else if (choice == 4 || strcmp(commands, "Redo") == 0)
        {
            redo();
        }
        else if (choice == 5 || strcmp(commands, "Print") == 0)
        {
            print_text();
        }
        else if (choice == 6 || strcmp(commands, "Save") == 0)
        {
            save();
        }
        else if (choice == 7 || strcmp(commands, "Open") == 0)
        {
            open_file();
        }
        else if (choice == 8 || strcmp(commands, "Exit") == 0)
        {
            printf(RED "\nExiting Text Editor 👋\n\n");
            break;
        }
        else
        {
            printf("\n❌ Invalid Command ❌\n\n");
        }
    }
}
