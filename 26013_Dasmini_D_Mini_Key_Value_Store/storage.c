#include <stdio.h>
#include "storage.h"
#include "kvstore.h"

//Bucket buckets[BUCKET_COUNT];

void save_store(void)
{
    FILE *fptr = fopen("store_db.txt", "w");
    //printf("saving...\n");
    if(fptr == NULL)
    {
        //printf("save failed!\n");
        return;
    }
    for(int i = 0; i < BUCKET_COUNT; i++)
    {
        Entry *temp = buckets[i].head;
        while(temp != NULL)
        {
            fprintf(fptr, "%s %s\n", temp->key, temp->value);
            temp = temp->link;
        }
    }
    //printf("saved\n");
    fclose(fptr);
}

void load_store()
{
    char line[100], key[45], value[45];
    FILE *fptr = fopen("store_db.txt", "r");
    if(fptr == NULL)
    {
        return;
    }
    while(fgets(line, sizeof(line), fptr) != NULL)
    {
        if(sscanf(line, "%44s %44[^\n]", key, value) == 2)
        {
            set(key, value);
        }
    }
    fclose(fptr);
    
}