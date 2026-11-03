#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#define SUCCESS 0
#define FAILURE -1

typedef struct node
{
    char *line;
    struct node *next;
    struct node *prev;
}Node;


Node *create_node(const char *text);
int append_node(Node **head, Node **tail, const char *text);
void display_document(Node *head);
Node *get_node(Node *head, int line_number);
int insert_line(Node **head, Node **tail,int line_number, const char *text);
int delete_line(Node **head, Node **tail, int line_number);


#endif