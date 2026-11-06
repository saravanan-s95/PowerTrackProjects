#include <stdio.h>
#include "kvstore.h"
#include "storage.h"
int main()
{
    init_store();
    
    while(1)
    {
        int output = get_command();
        if (output == EXIT)
        {
            save_store();
            free_store();
            break;
        }
            
        if (output == INVALID)
        {
            printf("Command not found!\n>");
        }
    }
    return 0;
}