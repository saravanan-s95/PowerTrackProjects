#ifndef RBT_H
#define RBT_H

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    RED,
    BLACK
} Color;

typedef struct RBNode {
    int key;
    Color color;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
} RBNode;

typedef struct {
    RBNode *root;
    RBNode *nil; /* Shared black sentinel representing every NIL leaf. */
} RBTree;

bool rbt_init(RBTree *tree);
void rbt_destroy(RBTree *tree);

bool rbt_insert(RBTree *tree, int key); /* false means duplicate or allocation failure */
bool rbt_delete(RBTree *tree, int key); /* false means key not found */
bool rbt_search(const RBTree *tree, int key);

bool rbt_minimum(const RBTree *tree, int *out_key);
bool rbt_maximum(const RBTree *tree, int *out_key);
bool rbt_delete_minimum(RBTree *tree, int *out_key);
bool rbt_delete_maximum(RBTree *tree, int *out_key);

void rbt_display(const RBTree *tree);
bool rbt_validate(const RBTree *tree);

#endif
