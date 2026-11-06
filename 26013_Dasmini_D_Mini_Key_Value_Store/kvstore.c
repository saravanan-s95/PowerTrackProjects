#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"
#include "kvstore.h"

Bucket buckets[BUCKET_COUNT];

/**
 * @brief initialized the hash table and loads the hashtable from the saved file
 */
void init_store(void)
{
    for(int i = 0; i < BUCKET_COUNT; i++)
    {
        buckets[i].head = NULL;
        buckets[i].store_count = 0;
    }
    load_store();
    printf(">");
}

/**
 * @brief Reads the line from terminal and calls the respective function
 */
int get_command()
{
    char command[100];
    char cmd[10];
    char arg1[45];
    char arg2[45];
    char arg3[45];
    if(fgets(command, sizeof(command), stdin) == NULL)
        return EXIT;
    int command_count = sscanf(command, "%9s %44s %44s %44s", cmd, arg1, arg2, arg3);
    if(strcmp(cmd, "SET") == 0)
    {
        if(command_count == 3)
        {
            set(arg1, arg2);
            printf("OK\n");
        }
        else
        {
            if(command_count == 4 && arg2[0] == '"')
            {
                int len = strlen(arg3);
                if(arg3[len-1] == '"')
                {
                    char value[100];
                    strcpy(value, &arg2[1]);
                    arg3[len-1] = '\0';
                    strcat(value, " ");
                    strcat(value, arg3);
                    set(arg1, value);
                    printf("OK\n");
                }
            }
            else
            {
                printf("ERROR : key and value is mandatory. Keys and values cannot contain spaces\n");
                printf(" try HELP for usage guide\nUsage : SET key value\n");
            }
        }
            printf(">");        
    }
    else if(strcmp(cmd, "GET") == 0)
    {
        if(command_count == 2)
        {
            const char *value = get(arg1);
            if(value == NULL)
                printf("(not found)\n");
            else
                printf("%s\n", value);
        }
        else
        {
            printf("ERROR : Key cannot contain spaces\nUsage : GET key\n");
        }
        printf(">");
    } 
    else if(command_count == 2 && strcmp(cmd, "DEL") == 0)
    {
        if(command_count == 2)
        {
            int status = del(arg1);
            if(status == 1)
                printf("OK\n");
            else
                printf("Failed\n");
        }
        else
            printf("ERROR : Key cannot contain spaces\nUsage : DEL key\n");
        printf(">");
    }
    else if(command_count == 1 && strcmp(cmd, "LIST") == 0)
    {
        if (command_count != 1)
            printf("Usage: LIST\n");
        else
            list();
    }
    else if(command_count == 1 && strcmp(cmd, "EXIT") == 0)
        return EXIT;
    else if(command_count == 1 && strcmp(cmd, "HELP") == 0)
        help();
    else
        return INVALID;
    return SUCCESS;
}

/**
 * @brief Frees all the memory after storing to the file.
 */
void free_store(void)
{
    for(int i = 0; i < BUCKET_COUNT; i++)
    {
        Entry *temp = buckets[i].head;
        Entry *next = temp;
        if(temp == NULL)
            continue;
        while(temp->link != NULL)
        {
            next = temp->link;
            free(temp);
            temp = next;
        }
        free(temp);
        buckets[i].head = NULL;
        buckets[i].store_count = 0;
    }
}

/**
 * @brief Finds the index for a specific key
 */
int get_index(const char key[])
{
    int index;
    unsigned int hash = 0;
    for(int i = 0; key[i] != '\0'; i++)
    {
        hash = hash * 31 + key[i];
    }
    index = hash % BUCKET_COUNT;

    return index;
}

/**
 * @brief places the key and value in the bucket's entries chain    
 */
void set(const char *key, const char *value)
{
    Entry *newEntry = malloc(sizeof(Entry));
    int index = get_index(key);
    if(buckets[index].head == NULL)
    {
        buckets[index].head = newEntry;
        snprintf(newEntry->key, KEY_SIZE, "%s", key);
        snprintf(newEntry->value, VALUE_SIZE, "%s", value);
        newEntry->link = NULL;
        buckets[index].store_count++;
    }
    else
    {
        Entry *temp = buckets[index].head;
        Entry *prev = temp;
        while(temp!=NULL)
        {
            if(strcmp(temp->key, key) == 0)
            {
                strcpy(temp->value, value);
                free(newEntry);
                return;
            }
            else
            {
                prev = temp;
                temp = temp-> link;
            }
        }
        prev->link = newEntry;
        snprintf(newEntry->key, KEY_SIZE, "%s", key);
        snprintf(newEntry->value, VALUE_SIZE, "%s", value);
        newEntry->link = NULL;
        buckets[index].store_count++;
    }
}

/**
 * @brief Searches the Buckets and returns the value
 */
const char *get(const char *key)
{
    int index = get_index(key);
    Entry *temp = buckets[index].head;
    while(temp != NULL)
    {
        if(strcmp(temp->key, key) == 0)
        {
            return temp->value;
        }
        temp = temp->link;
    }
    return NULL;
}

/**
 * @brief to delete a given key value from the Bucket
 */
int del(const char *key)
{
    int index = get_index(key);
    Entry *temp = buckets[index].head;
    Entry *prev = temp;
    while(temp != NULL)
    {
        if(strcmp(temp->key, key) == 0)
        {
            if(temp == buckets[index].head)
            {
                buckets[index].head = temp->link;  //If the key is the head entry, replace the head with the link.
            }
            else
            {
                prev->link = temp->link;
            }
            free(temp);
            buckets[index].store_count--;
            return 1;
        }
        prev = temp;
        temp = temp->link;
    }
    return 0;
}

/**
 * @brief List all the buckets entries
 */
void list(void)
{
    int isEmpty = 1;
    for(int i = 0; i < BUCKET_COUNT; i++)
    {
        Entry *temp = buckets[i].head;
        while(temp != NULL)
        {
            printf("%s : %s\n", temp->key, temp->value);
            temp = temp->link;
            isEmpty = 0;
        }
    }
    if(isEmpty)
    {
        printf("Nothing to Display!\n");
    }
    printf(">");
}

/**
 * @brief Prints all the commands and how to use them
 */
void help()
{
    printf("SET\n");
    printf("Needs 3 arguments. First is the command SET followed by a space and the key and value to be set.\n");
    printf("values with a space should be enclosed in quotes and only upto one space is allowed.\n");
    printf("Usage : SET key value\n");
    printf("\n");
    printf("GET\n");
    printf("Needs 2 arguments. First is the command GET followed by a space and the key whose value to get.\n");
    printf("Usage : GET key\n");
    printf("\n");
    printf("DEL\n");
    printf("Needs 2 arguments. First is the command DEL followed by a space and the key whose value to delete.\n");
    printf("Usage : DEL key\n");
    printf("\n");
    printf("LIST\n");
    printf("Needs 1 argument. The command LIST.\n");
    printf("Usage : LIST\n");
    printf("\n");
    printf("EXIT\n");
    printf("Needs 1 argument. The command EXIT to exit from the process.\n");
    printf("Usage : EXIT\n");
    printf("\n");
    printf("HELP\n");
    printf("Needs 1 argument. The command HELP, to get a list of all the commands and their usage and manual.\n");
    printf("Usage : HELP\n");
    printf("\n");
    printf(">");
}