#ifndef KVSTORE_H
#define KVSTORE_H

#define KEY_SIZE        50
#define VALUE_SIZE      50
#define BUCKET_COUNT    28
#define SUCCESS         1
#define EXIT            2
#define INVALID         3
         

typedef struct entry
{
    char key[KEY_SIZE];
    char value[VALUE_SIZE];
    struct entry *link;
} Entry;

typedef struct bucket
{
    Entry *head;
    int store_count;
    
} Bucket;

extern Bucket buckets[BUCKET_COUNT];

void init_store(void);
int get_command(void);
void free_store(void);
int get_index(const char key[]);
void set(const char *key, const char *value);
const char *get(const char *key);
int del(const char *key);
void list(void);
void help();

#endif