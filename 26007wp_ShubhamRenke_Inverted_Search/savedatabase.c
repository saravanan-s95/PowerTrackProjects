#include "inverter.h"

int savedatabase(hash_t *arr)
{
    char filename[25];
    printf("Enter file name to save database (Only .txt) : ");
    scanf("%s", filename);

    if (strstr(filename, ".txt") != NULL)
    {
        FILE *fptr = fopen(filename, "w");
        fprintf(fptr,"--------------------------------------------------------------------------\n");
        fprintf(fptr,"%-7s %-15s %-12s %-15s %s\n", "Index", "Word", "File Count", "Filename", "Word Count");
        fprintf(fptr,"--------------------------------------------------------------------------\n");
        for (int i = 0; i < 27; i++)
        {
            if (arr[i].link != NULL)
            {
                main_node *temp = arr[i].link;

                while (temp != NULL)
                {
                    sub_node *stemp = temp->sublink;
                    int is_first_file = 1; // Flag to track if it's the first file in the list

                    while (stemp != NULL)
                    {
                        if (is_first_file == 1)
                        {
                            // Print the main info AND the first file's info on the first line
                            fprintf(fptr, "%-7d %-15s %-12d %-15s %-10d\n", i, temp->word, temp->filecount, stemp->filename, stemp->word_count);
                            is_first_file = 0; // Turn off the flag for any extra files
                        }
                        else
                        {
                            // Print empty spaces for Index, Word, and FileCount to align the columns
                            fprintf(fptr, "%-7s %-15s %-12s %-15s %-10d\n", "", "", "", stemp->filename, stemp->word_count);
                        }

                        stemp = stemp->slink;
                    }

                    temp = temp->link; // Move to the next word in the hash table
                }
            }
        }
        fclose(fptr);
        printf("Database Save Succesfully !!!\n");
    }
    else
    {
        printf("Provide Valide File to save !!!\n");
        return FAILURE;
    }
}