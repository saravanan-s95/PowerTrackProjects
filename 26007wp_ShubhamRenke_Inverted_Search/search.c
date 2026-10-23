#include "inverter.h"
#include <string.h>

int searchdatabase(hash_t *arr, char *word)
{
    // find index
    int ind;

    if ((word[0] >= 'a' && word[0] <= 'z') || (word[0] >= 'A' && word[0] <= 'Z'))
    {
        ind = tolower(word[0]) - 'a';
    }
    else
    {
        ind = 26; // special characte
    }

    if(arr[ind].link == NULL)
    {
        return FAILURE;
    }
    else
    {
        main_node *m_temp = arr[ind].link;
        while(m_temp != NULL)
        {
            if(!strcmp(m_temp->word,word))
            {
                printf("%s is present in %d files.\n",m_temp->word,m_temp->filecount);
                sub_node *stemp = m_temp->sublink;
                while(stemp != NULL)
                {
                    printf("In %s %d Times\n",stemp->filename,stemp->word_count);
                    stemp = stemp->slink;
                }
                return SUCCESS;
            }
            m_temp = m_temp->link;
        }
        return FAILURE;
    }
}