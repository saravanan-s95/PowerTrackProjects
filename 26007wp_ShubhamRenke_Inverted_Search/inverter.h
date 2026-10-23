#ifndef inverter_h

#define inverter_h
#include<stdio.h>



#define SUCCESS  0
#define FAILURE  -1


// to store file name 
typedef struct f_node
{
    char filename[25];
    struct f_node *link;
}flist;

typedef struct s_node
{
    int word_count;
    char filename[25];
    struct s_node *slink;
}sub_node;

typedef struct m_node
{
    int filecount;
    char word[30];
    sub_node *sublink;
    struct m_node *link;
}main_node;

typedef struct h_node
{
    int index;
    struct m_node *link;
}hash_t;


int savedatabase(hash_t *arr);

int update_database(hash_t *arr, flist *head);

int searchdatabase(hash_t *arr, char *word);

int display(hash_t *arr);

int createdatabase(hash_t *arr, flist *head);

int Duplicate_find(char *name,flist *head);

int insert_last(flist **head , char *name);

int validateInput(flist * head,char * argv);

int File_present_or_not_empty(char * filename);

int print_list(flist *head);

#endif
