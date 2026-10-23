#include "inverter.h"
#include <stdlib.h>
#include<string.h>
#include<ctype.h>

char word[25];

int createdatabase(hash_t *arr, flist *head)
{
    flist *temp = head;

    while (temp != NULL)
    {
        /* Open file */
        FILE *fptr = fopen(temp->filename, "r");

        if (fptr == NULL)
        {
            printf("ERROR: Unable to open %s\n", temp->filename);
            temp = temp->link;
            continue;
        }

        /* Read every word from file */
        while (fscanf(fptr, "%24s", word) != EOF)
        {
            /* Find hash index */
            int ind;

            if ((word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A' && word[0] <= 'Z'))
            {
                ind = tolower(word[0]) - 'a';
            }
            else
            {
                ind = 26; // special character
            }

            /* Check valid index */
            if (ind < 0 || ind >= 27)
            {
                continue;
            }

            /*
             * Case 1:
             * No main node exists at this index
             */
            if (arr[ind].link == NULL)
            {
                /* Create main node */
                main_node *new_mainNode = malloc(sizeof(main_node));

                if (new_mainNode == NULL)
                {
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(new_mainNode->word, word);
                new_mainNode->filecount = 1;
                new_mainNode->link = NULL;

                /* Create sub node */
                sub_node *new_subNode =
                    malloc(sizeof(sub_node));

                if (new_subNode == NULL)
                {
                    free(new_mainNode);
                    fclose(fptr);
                    return FAILURE;
                }

                strcpy(new_subNode->filename, temp->filename);
                new_subNode->word_count = 1;
                new_subNode->slink = NULL;

                /* Connect sub node to main node */
                new_mainNode->sublink = new_subNode;

                /* Insert main node into hash table */
                arr[ind].link = new_mainNode;
            }

            /*
             * Case 2:
             * Main node(s) already exist
             */
            else
            {
                main_node *mainTemp = arr[ind].link;
                main_node *mainPrev = NULL;

                /*
                 * Search for the word in main-node list
                 */
                while (mainTemp != NULL)
                {
                    if (strcmp(mainTemp->word, word) == 0)
                    {
                        break;
                    }

                    mainPrev = mainTemp;
                    mainTemp = mainTemp->link;
                }

                /*
                 * Case 2A:
                 * Word does not exist
                 */
                if (mainTemp == NULL)
                {
                    /* Create main node */
                    main_node *new_mainNode =
                        malloc(sizeof(main_node));

                    if (new_mainNode == NULL)
                    {
                        fclose(fptr);
                        return FAILURE;
                    }

                    strcpy(new_mainNode->word, word);
                    new_mainNode->filecount = 1;
                    new_mainNode->link = NULL;

                    /* Create sub node */
                    sub_node *new_subNode =
                        malloc(sizeof(sub_node));

                    if (new_subNode == NULL)
                    {
                        free(new_mainNode);
                        fclose(fptr);
                        return FAILURE;
                    }

                    strcpy(new_subNode->filename,
                           temp->filename);

                    new_subNode->word_count = 1;
                    new_subNode->slink = NULL;

                    new_mainNode->sublink = new_subNode;

                    /*
                     * Add main node at end
                     * of main-node list
                     */
                    mainPrev->link = new_mainNode;
                }

                /*
                 * Case 2B:
                 * Word already exists
                 */
                else
                {
                    sub_node *subTemp = mainTemp->sublink;
                    sub_node *subPrev = NULL;

                    /*
                     * Search whether current file
                     * already contains this word
                     */
                    while (subTemp != NULL)
                    {
                        if (strcmp(subTemp->filename, temp->filename) == 0)
                        {
                            break;
                        }

                        subPrev = subTemp;
                        subTemp = subTemp->slink;
                    }

                    /*
                     * Word already exists in
                     * current file
                     */
                    if (subTemp != NULL)
                    {
                        subTemp->word_count++;
                    }

                    /*
                     * Word exists, but this is
                     * a new file
                     */
                    else
                    {
                        sub_node *new_subNode =
                            malloc(sizeof(sub_node));

                        if (new_subNode == NULL)
                        {
                            fclose(fptr);
                            return FAILURE;
                        }

                        strcpy(new_subNode->filename, temp->filename);

                        new_subNode->word_count = 1;
                        new_subNode->slink = NULL;

                        /*
                         * Add new sub node
                         */
                        subPrev->slink = new_subNode;

                        /*
                         * Increment number of files
                         */
                        mainTemp->filecount++;
                    }
                }
            }
        }

        fclose(fptr);

        temp = temp->link;
    }

    return SUCCESS;
}