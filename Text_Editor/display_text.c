#include "display_text.h"

void display_text(Node *head)
{
    if(head == NULL)
    {
        printf("\nEditor is Empty\n");
        return;
    }

    Node *temp = head;
    int line = 1;

    printf("\n---------------------------------\n");
    printf("        TEXT EDITOR\n");
    printf("---------------------------------\n");

    while(temp != NULL)
    {
        printf("%d. %s\n", line, temp->line);

        temp = temp->next;
        line++;
    }

    printf("---------------------------------\n");
}



Node *search_text(Node *head, char *word)
{
    Node *temp = head;
    int line = 1;

    while(temp != NULL)
    {
        if(strstr(temp->line, word) != NULL)
        {
            printf("\nText Found at Line %d\n", line);

            return temp;
        }

        temp = temp->next;
        line++;
    }

    printf("\nText Not Found\n");

    return NULL;
}



void replace_text(Node *head, char *old_word, char *new_word)
{
    Node *temp = head;

    while(temp != NULL)
    {
        char *pos = strstr(temp->line, old_word);

        if(pos != NULL)
        {
            char buffer[MAX_LINE_LENGTH];

            /* Copy the text before the old word */
            int len = pos - temp->line;
            strncpy(buffer, temp->line, len);
            buffer[len] = '\0';

            /* Add the new word */
            strcat(buffer, new_word);

            /* Add the remaining text */
            strcat(buffer, pos + strlen(old_word));

            /* Store the updated line */
            strcpy(temp->line, buffer);

            printf("Replacement Successful\n");
            return;
        }

        temp = temp->next;
    }

    printf("Word Not Found\n");
}