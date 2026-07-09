/*👤NAME        : THARSHINI S
  📅DATE        : 09-12-2025
  📝DESCRIPTION : INVERTED SEARCH - HEADER FILE */ 
  
#ifndef INVERTED_H
#define INVERTED_H

#include<stdio.h>
#include<stdlib.h>

#define SIZE    256 //Hash table size (a-z)
#define MAXWORD 30 //Maximum length of a word
#define MAXFILE 50 //Maximum length of a filename

#define SUCCESS 0
#define FAILURE -1
#define FILE_NOT_FOUND -2
#define FILE_EMPTY -3
#define FILE_REPEATED -4

/*Filenode stores a filename and how many times the word appears*/
typedef struct FileNode
{
    char fname[MAXFILE];
    int word_count;
    struct FileNode *f_link;
}FileNode;

/*Wordnode stores a word ,number of files it appears in and linked list of corresponding filenodes*/
typedef struct WordNode
{
    char word[MAXWORD];
    int file_count;
    struct FileNode *f_link;
    struct WordNode *w_link;
}WordNode;

/*In hash table each index stores a linked list of wordnodes */
typedef struct Htable
{
    int index;
    WordNode *head;
}Htable;

/*Stores input filenames before creating database*/
typedef struct Inputfile
{
    char fname[MAXFILE];
    struct Inputfile *link;
}Inputfile;

/*----Function Prototypes----*/

int validate_args(int argc,char *argv[],Inputfile **list);
int is_textfile(const char *fname);

int create_database(Htable *table, Inputfile *list);
int update_database(Htable *table,char *fname,Inputfile **list);
void display_database(Htable *table);
int save_database(Htable *table,Inputfile *list,char *name);
int search_database(Htable *table,char *word);

/*Helper functions*/
int hash_key(const char *word);
void insert_word_to_hashtable(Htable *table,char *word, char *fname, int index);
int is_file_in_hashtable(Htable *table, const char *fname);
int is_file_in_list(Inputfile *list, const char *fname);

/*Cleanup functions*/
void free_filelist(Inputfile **list);
void free_database(Htable *table);

#endif