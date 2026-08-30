#include "types.h"

/* Singly LL for storing file names */
typedef struct node
{
    char f_name[100];
    struct node *link;
} File_List;

/* Stack for handling parantheses */
typedef struct s
{
    int top;
    char *arr;
} Stack;

/* Function Prototypes */

/* Function to store all files provided in CLA into a Linked List */
Status storing_cla_fnames_to_LL(char *argv[], File_List **head);

/* Function to perform insert last operation for storing new file node */
Status insert_last(File_List **head, File_List *new_node);

/* Function to validate file name */
Status validate_fname(char f_name[]);

/* Function that performs lexical analysis */
Status lexical_analyser(File_List **head, Stack *stk);

/* Function to check whether it is a word or not */
Status isword(char word[]);

/* Function to check whether word is keyword or not */
Status is_keyword(char word[]);

/* Function to check whether char is symbol or not */
Status is_symbol(char ch);

/* Function to handle parantheses */
Status parantheses_handler(Stack *stk, char ch, char *f_name);