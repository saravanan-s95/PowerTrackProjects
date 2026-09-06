 #ifndef INS
 #define INS

 #include<stdio.h>
 #include<stdlib.h>
 #include<ctype.h>
 #include<string.h>

 typedef struct sub_node{
    int wc;
    char *fname;
    struct sub_node *slink;
 }snode;

 typedef struct main_node
 {
    char word[20];
    int filecount;
    struct main_node *mlink;
    struct sub_node *slink;
 }mnode;

 typedef struct node
 {
    char filename[20];
    struct node *link;
 }Slist;


 void display_database(mnode *arr[]);

 void create_database(mnode *arr[],Slist *head);

 int read_and_validate(int argc,char *argv[],Slist **head);

 void search_database(mnode *arr[]);

 void update_database(mnode *arr[],Slist **head);
 
 void insert_at_last(Slist **head, char *str );

 
 void save_database(mnode *arr[]);
 
 #endif 