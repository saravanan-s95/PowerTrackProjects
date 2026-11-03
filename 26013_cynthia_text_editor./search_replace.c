#include <stdio.h>
#include <string.h>
#include "search_replace.h"
#include <stdlib.h>

int search_text(Node *head, const char *search)
{
    Node *temp = head;
    int line_number = 1;
    int found = 0;

    if (search == NULL || search[0] == '\0')
        return -1;

    while (temp != NULL)
    {
        if (strstr(temp->line, search) != NULL)
        {
            printf("Found \"%s\" in line %d: %s\n",
                   search, line_number, temp->line);
            found = 1;
        }

        temp = temp->next;
        line_number++;
    }

    if (!found)
        return 0;

    return 1;
}



int replace_text(Node *head, const char *old_text,const char *new_text)
{
    Node *temp = head;
    int replacements = 0;
    int i, j, k;
    size_t old_len, new_len;

    if (old_text == NULL || new_text == NULL ||
        old_text[0] == '\0')
        return -1;

    old_len = strlen(old_text);
    new_len = strlen(new_text);

    while (temp != NULL)
    {
        char *match;

        while ((match = strstr(temp->line, old_text)) != NULL)
        {
            size_t prefix_len = (size_t)(match - temp->line);
            size_t suffix_len = strlen(match + old_len);
            size_t result_len = prefix_len + new_len + suffix_len;
            char *updated = malloc(result_len + 1);

            if (updated == NULL)
                return -1;

            /* Copy the characters before the match. */
            for (i = 0; i < (int)prefix_len; i++)
                updated[i] = temp->line[i];

            /* Copy the replacement text. */
            for (j = 0; j < (int)new_len; j++)
                updated[i + j] = new_text[j];

            /* Copy the characters after the match. */
            for (k = 0; k <= (int)suffix_len; k++)
                updated[i + j + k] = match[old_len + k];

            free(temp->line);
            temp->line = updated;

            replacements++;
        }

        temp = temp->next;
    }

    return replacements;
}
