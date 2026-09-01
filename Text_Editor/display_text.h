#ifndef DISPLAY_SEARCH_H
#define DISPLAY_SEARCH_H

#include "text_editor.h"

/* Display Functions */
void display_text(Node *head);

/* Search Functions */
Node *search_text(Node *head, char *word);

/* Replace Functions */
void replace_text(Node *head, char *old_word, char *new_word);

#endif