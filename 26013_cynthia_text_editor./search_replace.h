#ifndef SEARCH_REPLACE_H
#define SEARCH_REPLACE_H

#include "text_editor.h"

int search_text(Node *head, const char *search);
int replace_text(Node *head, const char *old_text,const char *new_text);


#endif