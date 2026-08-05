#include "rbt.h"


#include <stdio.h>

#define SPACE_COUNT 8

void print_tree_structured(node* root, int space, char child_type) {
    if (root == NULL) {
        return;
    }
    space += SPACE_COUNT;//indentation for next level
    print_tree_structured(root->right, space, 'R');//processing right sub tree
    // printing curr node 
    printf("\n");
    for (int i = SPACE_COUNT; i < space; i++) {
        printf(" ");
    }

    // color tags ([R] = RED, [B] = BLACK)
    if (root->color == 1) {
        printf("%d[R]\n", root->data);
    } else {
        printf("%d[B]\n", root->data);
    }

    print_tree_structured(root->left, space, 'L');//left subtree
}

void print_tree(REDBLACKTREE* tree) {
    if (tree == NULL || tree->root == NULL) {
        printf("The tree is currently empty.\n");
        return;
    }
    printf("\n--- Red-Black Tree Layout (Read Left to Right) ---\n");
    print_tree_structured(tree->root, 0, 'O');
    printf("--------------------------------------------------\n");
}
