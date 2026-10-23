#include"inverter.h"

int display(hash_t *arr)
{
    printf("--------------------------------------------------------------------------\n");
    printf("%-7s %-15s %-12s %-15s %s\n", "Index", "Word", "File Count", "Filename", "Word Count");
    printf("--------------------------------------------------------------------------\n");

    for(int i = 0; i < 27; i++)
    {
        if(arr[i].link != NULL)
        {
            main_node *temp = arr[i].link;

            while(temp != NULL)
            {
                sub_node *stemp = temp->sublink;
                int is_first_file = 1; // Flag to track if it's the first file in the list
                
                while(stemp != NULL)
                {
                    if (is_first_file == 1)
                    {
                        // Print the main info AND the first file's info on the first line
                        printf("%-7d %-15s %-12d %-15s %-10d\n", 
                               i, temp->word, temp->filecount, stemp->filename, stemp->word_count);
                        
                        is_first_file = 0; // Turn off the flag for any extra files
                    }
                    else
                    {
                        // Print empty spaces for Index, Word, and FileCount to align the columns
                        printf("%-7s %-15s %-12s %-15s %-10d\n", 
                               "", "", "", stemp->filename, stemp->word_count);
                    }
                    
                    stemp = stemp->slink;
                }
                
                temp = temp->link; // Move to the next word in the hash table
            }
        }
    }
    
    printf("--------------------------------------------------------------------------\n");
    return SUCCESS;
}

