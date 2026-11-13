/*
 * Project Name : Red-Black Tree Implementation
 *
 * Description  : Implements a self-balancing Binary Search Tree
 *                using Red-Black Tree properties. Supports
 *                insertion, deletion, searching, finding minimum
 *                and maximum values, displaying the tree, and
 *                validating Red-Black Tree properties.
 *
 * Author       : Sheeba Insily R
 * Batch Number : 26013
 * File Name    : main.c
 *
 * Concepts Used:
 * - C Programming
 * - Data Structures
 * - Binary Search Trees
 * - Red-Black Tree
 * - Pointers and Dynamic Memory Allocation
 * - Functions and Input Validation
 */
#include "rbt.h"
#include <stdio.h>
#include <limits.h>

static int read_int(const char *prompt, int *value)
{
    int ch;
    printf("%s", prompt);
    if (scanf("%d", value) != 1) {
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        puts("Invalid input. Please enter an integer.");
        return 0;
    }
    while ((ch = getchar()) != '\n' && ch != EOF) {}
    return 1;
}

static void report_validity(const RBTree *tree)
{
    printf("Red-Black properties: %s\n",
           rbt_validate(tree) ? "VALID" : "INVALID");
}

int main(void)
{
    RBTree tree = {0};
    if (!rbt_init(&tree)) {
        fputs("Failed to initialize the tree (out of memory).\n", stderr);
        return 1;
    }

    int choice, key;
    for (;;) {
        puts("\n========== RED-BLACK TREE ==========");
        puts("1. Insert");
        puts("2. Delete");
        puts("3. Search");
        puts("4. Find Minimum");
        puts("5. Delete Minimum");
        puts("6. Find Maximum");
        puts("7. Delete Maximum");
        puts("8. Display");
        puts("9. Validate Red-Black Properties");
        puts("0. Exit");

        if (!read_int("Enter choice: ", &choice))
            continue;

        switch (choice) {
        case 1:
            if (!read_int("Enter value to insert: ", &key))
                break;
            if (rbt_search(&tree, key)) {
                printf("%d already exists; duplicates are not inserted.\n", key);
            } else if (rbt_insert(&tree, key)) {
                printf("Inserted %d.\n", key);
                rbt_display(&tree);
                report_validity(&tree);
            } else {
                puts("Insertion failed (possibly out of memory).");
            }
            break;

        case 2:
            if (!read_int("Enter value to delete: ", &key))
                break;
            if (rbt_delete(&tree, key)) {
                printf("Deleted %d.\n", key);
                rbt_display(&tree);
                report_validity(&tree);
            } else {
                printf("%d was not found.\n", key);
            }
            break;

        case 3:
            if (!read_int("Enter value to search: ", &key))
                break;
            printf("%d %s in the tree.\n", key,
                   rbt_search(&tree, key) ? "was found" : "was not found");
            break;

        case 4:
            if (rbt_minimum(&tree, &key))
                printf("Minimum value: %d\n", key);
            else
                puts("Tree is empty.");
            break;

        case 5:
            if (rbt_delete_minimum(&tree, &key)) {
                printf("Deleted minimum value: %d\n", key);
                rbt_display(&tree);
                report_validity(&tree);
            } else {
                puts("Tree is empty.");
            }
            break;

        case 6:
            if (rbt_maximum(&tree, &key))
                printf("Maximum value: %d\n", key);
            else
                puts("Tree is empty.");
            break;

        case 7:
            if (rbt_delete_maximum(&tree, &key)) {
                printf("Deleted maximum value: %d\n", key);
                rbt_display(&tree);
                report_validity(&tree);
            } else {
                puts("Tree is empty.");
            }
            break;

        case 8:
            rbt_display(&tree);
            break;

        case 9:
            report_validity(&tree);
            break;

        case 0:
            rbt_destroy(&tree);
            puts("Tree memory released. Goodbye.");
            return 0;

        default:
            puts("Invalid choice. Enter a number from 0 to 9.");
        }
    }
}
